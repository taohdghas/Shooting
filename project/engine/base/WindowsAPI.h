#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace MyEngine {

    // WindowsAPI関連のクラス
    /// <summary>
    /// Windowsアプリケーションのウィンドウ生成・管理・破棄
    /// ウィンドウプロシージャによるメッセージ処理
    /// Windowsメッセージループの管理
    /// ウィンドウハンドルやインスタンスハンドル等の取得インターフェースの提供
    /// ウィンドウクラス情報の管理
    /// シングルトンによるインスタンス管理
    /// </summary>
    class WindowsApi {
    public:
        /// <summary>
        /// ウィンドウプロシージャ（静的コールバック）
        /// <param name="hwnd">ウィンドウハンドル</param>
        /// <param name="msg">メッセージID</param>
        /// <param name="wparam">追加情報1</param>
        /// <param name="lparam">追加情報2</param>
        /// <returns>メッセージ処理結果（LRESULT）</returns>
        /// </summary>
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        /// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>WindowsApiのインスタンス（ポインタ）</returns>
        /// </summary>
        static WindowsApi* GetInstance();

        /// <summary>
        /// 初期化処理
        /// <returns>なし</returns>
        /// </summary>
        void Initialize();

        /// <summary>
        /// 終了処理
        /// <returns>なし</returns>
        /// </summary>
        void Finalize();

        /// <summary>
        /// Windows メッセージをポーリングして処理
        /// <returns>終了要求時はfalse、継続時はtrue</returns>
        /// </summary>
        bool ProcessMessage();

        /// <summary>
        /// ウィンドウハンドルを取得
        /// <returns>ウィンドウハンドル（HWND）</returns>
        /// </summary>
        HWND GetHwnd() const { return hwnd_; }

        /// <summary>
        /// インスタンスハンドルを取得
        /// <returns>インスタンスハンドル（HINSTANCE）</returns>
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

        // ウインドウハンドル
        HWND hwnd_ = nullptr;

        // ウィンドウクラスの設定
        WNDCLASS wc_{};
    };
}