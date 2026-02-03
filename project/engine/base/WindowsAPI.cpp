#pragma comment (lib,"winmm.lib")
#include "WindowsApi.h"

#ifdef USE_IMGUI
#include <externals/imgui/imgui_impl_win32.h>
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

namespace MyEngine {

    std::unique_ptr<WindowsApi> WindowsApi::instance_ = nullptr;

    // ウィンドウプロシージャ
    LRESULT CALLBACK WindowsApi::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef USE_IMGUI
        // ImGuiのメッセージ処理
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
            return true;
        }
#endif
        // ゲーム固有のメッセージ処理
        switch (msg) {
        case WM_DESTROY:
            // ウィンドウ破棄時、アプリ終了をOSに通知
            PostQuitMessage(0);
            return 0;
        }

        // 標準のメッセージ処理
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    // シングルトンインスタンス取得
    WindowsApi* WindowsApi::GetInstance() {
        if (!instance_) {
            instance_ = std::make_unique<WindowsApi>();
        }
        return instance_.get();
    }

    void WindowsApi::Initialize() {
        // COMライブラリ初期化
        HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

        // システムタイマーの精度を上げる
        timeBeginPeriod(1);

        // ウィンドウクラスの設定
        wc_.lpfnWndProc = WindowProc;             // ウィンドウプロシージャ
        wc_.lpszClassName = L"CG2WindowClass";    // クラス名
        wc_.hInstance = GetModuleHandle(nullptr); // インスタンスハンドル
        wc_.hCursor = LoadCursor(nullptr, IDC_ARROW); // カーソル設定

        // ウィンドウクラスを登録
        RegisterClass(&wc_);

        // クライアント領域のサイズを設定
        RECT window_rect = { 0, 0, kClientWidth, kClientHeight };

        // 実際のウィンドウサイズに調整
        AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, false);

        // ウィンドウ生成
        hwnd_ = CreateWindow(
            wc_.lpszClassName,
            L"CG2",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            window_rect.right - window_rect.left,
            window_rect.bottom - window_rect.top,
            nullptr,
            nullptr,
            wc_.hInstance,
            nullptr
        );

        // ウィンドウ表示
        ShowWindow(hwnd_, SW_SHOW);
    }

    void WindowsApi::Update() {
        // 今回は何も処理なし
    }

    void WindowsApi::Finalize() {
        // ウィンドウを閉じる
        CloseWindow(hwnd_);

        // COMライブラリの解放
        CoUninitialize();
    }

    bool WindowsApi::ProcessMessage() {
        MSG msg{};

        // メッセージが存在すれば取得して処理
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // WM_QUITが来ていればtrueを返す
        return msg.message == WM_QUIT;
    }
}