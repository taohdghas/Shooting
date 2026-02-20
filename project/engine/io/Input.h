#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Windows.h"
#include <dinput.h>
#include <wrl.h>
#include "WindowsAPI.h"
#include <memory>

namespace MyEngine {

    // 入力関連クラス
    /// <summary>
    /// DirectInputを用いたキーボードおよびマウス入力の初期化・終了・毎フレーム更新処理
    /// キー・マウスボタンの押下／トリガー状態の検出と取得インターフェースの提供
    /// 入力状態のクリアやマウス座標の取得
    /// WindowsAPIとの連携による入力デバイス管理
    /// シングルトンによるインスタンス管理
    /// </summary>
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

        /// <summary>
        /// マウス左ボタンが押されているか
        /// </summary>
        bool IsMouseLeftPressed();

        /// <summary>
        /// マウス左ボタンがトリガーか
        /// </summary>
        bool IsMouseLeftTriggered();

        /// <summary>
        /// マウス右ボタンが押されているか
        /// </summary>
        bool IsMouseRightPressed();

        /// <summary>
        /// マウス右ボタンがトリガーか
        /// </summary>
        bool IsMouseRightTriggered();

        /// <summary>
        /// マウスのクライアント座標を取得
        /// </summary>
        POINT GetMousePosition();

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

		// マウス左ボタンの前回状態
        bool mouse_left_prev_ = false;
		// マウス左ボタンの現在状態
        bool mouse_left_curr_ = false;
        // マウス右ボタンの前回状態
        bool mouse_right_prev_ = false;
        // マウス右ボタンの現在状態
        bool mouse_right_curr_ = false;
    };
}