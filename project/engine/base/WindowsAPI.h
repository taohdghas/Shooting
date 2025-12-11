#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace MyEngine {

    // WindowsAPI関連のクラス
    class WindowsApi {
    public:
        /// <summary>
        /// ウィンドウプロシージャ（静的コールバック）
        /// </summary>
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static WindowsApi* GetInstance();

        /// <summary>
        /// 初期化処理を行う。
        /// </summary>
        void Initialize();

        /// <summary>
        /// 毎フレーム更新処理（現在は未使用／空実装）。
        /// </summary>
        void Update();

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// Windows メッセージをポーリングして処理する。
        /// </summary>
        bool ProcessMessage();

        // getter

        /// <summary>
        /// ウィンドウハンドルを取得する。
        /// </summary>
        HWND GetHwnd() const { return hwnd_; }

        /// <summary>
        /// インスタンスハンドルを取得する。
        /// </summary>
        HINSTANCE GetHinstance() const { return wc_.hInstance; }

        // クライアント領域のサイズ
        static const int32_t kClientWidth = 1280;
        static const int32_t kClientHeight = 720;
    public:
        WindowsApi() = default;
        ~WindowsApi() = default;
    private:
        static std::unique_ptr<WindowsApi> instance_;

        WindowsApi* windows_api_ = nullptr;

        // ウインドウハンドル
        HWND hwnd_ = nullptr;

        // ウィンドウクラスの設定
        WNDCLASS wc_{};
    };
}