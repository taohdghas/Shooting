#pragma comment (lib,"winmm.lib")
#include "WindowsAPI.h"
#include <externals/imgui/imgui_impl_win32.h>

#pragma comment (lib,"winmm.lib")
#include "WindowsAPI.h"
#include <externals/imgui/imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowsAPI* WindowsAPI::instance = nullptr;

// ウィンドウプロシージャ
LRESULT CALLBACK WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    // ImGuiのメッセージ処理
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }

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
WindowsAPI* WindowsAPI::GetInstance() {
    if (instance == nullptr) {
        instance = new WindowsAPI;
    }
    return instance;
}

void WindowsAPI::Initialize() {
    // COMライブラリ初期化
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

    // システムタイマーの精度を上げる
    timeBeginPeriod(1);

    // ウィンドウクラスの設定
    wc.lpfnWndProc = WindowProc;           // ウィンドウプロシージャ
    wc.lpszClassName = L"CG2WindowClass";  // クラス名
    wc.hInstance = GetModuleHandle(nullptr); // インスタンスハンドル
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW); // カーソル設定

    // ウィンドウクラスを登録
    RegisterClass(&wc);

    // クライアント領域のサイズを設定
    RECT wrc = { 0, 0, kClientWidth, kClientHeight };

    // 実際のウィンドウサイズに調整
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    // ウィンドウ生成
    hwnd = CreateWindow(
        wc.lpszClassName,
        L"CG2",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr
    );

    // ウィンドウ表示
    ShowWindow(hwnd, SW_SHOW);
}

void WindowsAPI::Update() {
    // 今回は何も処理なし
}

void WindowsAPI::Finalize() {
    // ウィンドウを閉じる
    CloseWindow(hwnd);
    // COMライブラリの解放
    CoUninitialize();
}

bool WindowsAPI::ProcessMessage() {
    MSG msg{};

    // メッセージが存在すれば取得して処理
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // WM_QUITが来ていればtrueを返す
    return msg.message == WM_QUIT;
}
