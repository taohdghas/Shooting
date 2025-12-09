#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace MyEngine {

    std::unique_ptr<Input> Input::instance_ = nullptr;

    // シングルトンインスタンス取得
    Input* Input::GetInstance() {
        if (!instance_) {
            instance_ = std::make_unique<Input>();
        }
        return instance_.get();
    }

    void Input::Initialize(WindowsApi* windows_api) {
        // 外部から渡されたWinAPIインスタンスを保持
        windows_api_ = windows_api;

        HRESULT result;

        // DirectInputインスタンス生成
        result = DirectInput8Create(
            windows_api_->GetHinstance(),
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            reinterpret_cast<void**>(direct_input_.GetAddressOf()),
            nullptr
        );
        assert(SUCCEEDED(result));

        // キーボードデバイス生成
        result = direct_input_->CreateDevice(GUID_SysKeyboard, keyboard_device_.GetAddressOf(), NULL);
        assert(SUCCEEDED(result));

        // 入力データ形式を設定（標準キーボード形式）
        result = keyboard_device_->SetDataFormat(&c_dfDIKeyboard);
        assert(SUCCEEDED(result));

        // 排他制御レベルの設定（フォアグラウンド＋非排他＋Winキー無効）
        result = keyboard_device_->SetCooperativeLevel(
            windows_api_->GetHwnd(),
            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
        );
        assert(SUCCEEDED(result));
    }

    void Input::Finalize() {
        // 必要な終了処理
        instance_.reset();
    }

    void Input::Update() {
        // 前回のキー入力状態を保存
        memcpy(prev_key_state_, key_state_, sizeof(key_state_));

        // キーボードデバイスから最新の入力状態を取得
        keyboard_device_->Acquire();
        keyboard_device_->GetDeviceState(sizeof(key_state_), key_state_);
    }

    void Input::ClearInput() {
        // キー状態配列をゼロクリア
        memset(key_state_, 0, sizeof(key_state_));
        memset(prev_key_state_, 0, sizeof(prev_key_state_));
    }

    bool Input::IsKeyPressed(BYTE key_code) {
        // 指定キーが押されていれば true
        return key_state_[key_code] != 0;
    }

    bool Input::IsKeyTriggered(BYTE key_code) {
        // 指定キーが押された瞬間（前回は押されていなかった）なら true
        return prev_key_state_[key_code] == 0 && key_state_[key_code] != 0;
    }
}