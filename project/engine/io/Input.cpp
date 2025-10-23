#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::instance = nullptr;

// シングルトンインスタンス取得
Input* Input::GetInstance() {
    if (instance == nullptr) {
        instance = new Input;
    }
    return instance;
}

void Input::Initialize(WindowsAPI* windowsAPI) {
    // 外部から渡されたWinAPIインスタンスを保持
    this->windowsAPI_ = windowsAPI;

    HRESULT result;

    // DirectInputインスタンス生成
    result = DirectInput8Create(
        windowsAPI->GetHInstance(),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&directInput,
        nullptr
    );
    assert(SUCCEEDED(result));

    // キーボードデバイス生成
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // 入力データ形式を設定（標準キーボード形式）
    result = keyboard->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(result));

    // 排他制御レベルの設定（フォアグラウンド＋非排他＋Winキー無効）
    result = keyboard->SetCooperativeLevel(
        windowsAPI->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
    );
    assert(SUCCEEDED(result));
}

// 終了処理
void Input::Finalize() {
    delete instance;
    instance = nullptr;
}

void Input::Update() {
    // 前回のキー入力状態を保存
    memcpy(keyPre, key, sizeof(key));

    // キーボードデバイスから最新の入力状態を取得
    keyboard->Acquire();
    keyboard->GetDeviceState(sizeof(key), key);
}

// 1フレーム分の入力をクリア
void Input::ClearInput() {
    memset(key, 0, sizeof(key));
    memset(keyPre, 0, sizeof(keyPre));
}

// 指定キーが押されていればtrue
bool Input::PushKey(BYTE keyNumber) {
    return key[keyNumber] != 0;
}

// 指定キーが押された瞬間（前回は押されていなかった）ならtrue
bool Input::TriggerKey(BYTE keyNumber) {
    return keyPre[keyNumber] == 0 && key[keyNumber] != 0;
}
