#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>

namespace MyEngine {

    // WindowsAPI関連のクラス
    class WindowsApi {
    public:
        /// <summary>
        /// ウィンドウプロシージャ（静的コールバック）。
        /// - ImGui の Win32 ハンドラにまず渡し、処理済みならそれを優先する。
        /// - WM_DESTROY を受け取った場合は <c>PostQuitMessage(0)</c> を呼んでアプリ終了を通知する。
        /// - それ以外は既定の処理を <c>DefWindowProc</c> に委ねる。
        /// </summary>
        /// <param name="hwnd">ウィンドウハンドル。</param>
        /// <param name="msg">メッセージコード。</param>
        /// <param name="wparam">メッセージ固有の追加情報。</param>
        /// <param name="lparam">メッセージ固有の追加情報。</param>
        /// <returns>処理結果の LRESULT。</returns>
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// - 初回呼び出し時にインスタンスを生成して返す。
        /// </summary>
        /// <returns>WindowsApi の唯一のインスタンスへのポインタ。</returns>
        static WindowsApi* GetInstance();

        /// <summary>
        /// 初期化処理を行う。
        /// - COM の初期化（<c>CoInitializeEx</c>）、システムタイマーの分解能設定（<c>timeBeginPeriod(1)</c>）を行う。
        /// - ウィンドウクラス設定、登録、ウィンドウ生成、表示を行う。
        /// </summary>
        void Initialize();

        /// <summary>
        /// 毎フレーム更新処理（現在は未使用／空実装）。
        /// </summary>
        void Update();

        /// <summary>
        /// 終了処理を行う。
        /// - ウィンドウを閉じ、COM をアンロードする（<c>CoUninitialize</c>）。
        /// </summary>
        void Finalize();

        /// <summary>
        /// Windows メッセージをポーリングして処理する。
        /// - <c>PeekMessage</c> でメッセージを取得し、TranslateMessage/DispatchMessage を行う。
        /// - WM_QUIT を検出した場合は true を返す（アプリ終了要求）。
        /// </summary>
        /// <returns>WM_QUIT を受け取っていれば true、そうでなければ false。</returns>
        bool ProcessMessage();

        // getter
        HWND GetHwnd() const { return hwnd_; }
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