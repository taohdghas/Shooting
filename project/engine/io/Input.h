#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Windows.h"
#include <dinput.h>
#include <wrl.h>
#include "WindowsAPI.h"
#include <memory>

namespace MyEngine {

    // 入力関連クラス
    class Input {
    public:
        // ComPtrのエイリアス
        template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static Input* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// </summary>
        void Initialize(WindowsApi* windows_api);

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// </summary>
        void Update();

        /// <summary>
        /// 1フレーム分の入力をクリアする（入力を無視する）。
        /// </summary>
        void ClearInput();

        /// <summary>
        /// 指定キーが押されているかをチェックする。
        /// </summary>
        bool IsKeyPressed(BYTE key_code);

        /// <summary>
        /// 指定キーがトリガー（前フレームは未押下、今回押下）かをチェックする。
        /// </summary>
        bool IsKeyTriggered(BYTE key_code);

    public:
        Input() = default;
        ~Input() = default;

    private:
        static std::unique_ptr<Input> instance_;

        // キーボードのデバイス
        ComPtr<IDirectInputDevice8> keyboard_device_;
        // 全キーの状態
        BYTE key_state_[256] = {};
        // 前回のキーの状態
        BYTE prev_key_state_[256] = {};
        // DirectInputのインスタンス
        ComPtr<IDirectInput8> direct_input_;
        // WindowsAPI
        WindowsApi* windows_api_ = nullptr;
    };
}