#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::instance = nullptr;

// シングルトンインスタンス取得
ImGuiManager* ImGuiManager::GetInstance() {
    if (instance == nullptr) {
        instance = new ImGuiManager;
    }
    return instance;
}

// 終了処理
void ImGuiManager::Finalize() {
#ifdef USE_IMGUI
    // ImGuiのDirectX12とWin32用をシャットダウン
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif
    delete instance;
    instance = nullptr;
}

// 初期化
void ImGuiManager::Initialize([[maybe_unused]] WindowsAPI* windowsAPI, DirectXBase* directxBase, [[maybe_unused]] SrvManager* srvmanager) {
#ifdef USE_IMGUI
    windowsAPI_ = windowsAPI;
    directxbase_ = directxBase;
    srvmanager_ = srvmanager;

    // SRV用のインデックスを取得
    index = srvmanager_->Allcate();

    // ImGuiのコンテキスト生成
    ImGui::CreateContext();

    // スタイルをダークテーマに設定
    ImGui::StyleColorsDark();

    // 日本語フォントを読み込む
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(
        "resources/font/07にくまるフォント.otf",
        15.0f,
        nullptr,
        io.Fonts->GetGlyphRangesJapanese()
    );

    // Win32用初期化
    ImGui_ImplWin32_Init(windowsAPI_->GetHwnd());

    // DirectX12用初期化
    ImGui_ImplDX12_Init(
        directxbase_->Getdevice().Get(),
        static_cast<int>(directxbase_->GetSwapChainResourcesNum()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srvmanager_->GetDescriptorHeap(),
        srvmanager_->GetCPUDescriptorHandle(index),
        srvmanager_->GetGPUDescriptorHandle(index)
    );
#endif
}

// ImGui受付開始（フレーム開始）
void ImGuiManager::Begin() {
#ifdef USE_IMGUI
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif
}

// ImGui受付終了（描画準備）
void ImGuiManager::End() {
#ifdef USE_IMGUI
    ImGui::Render();
#endif
}

// 実際に画面へ描画
void ImGuiManager::Draw() {
#ifdef USE_IMGUI
    ID3D12GraphicsCommandList* commandList = directxbase_->Getcommandlist().Get();

    // デスクリプタヒープを設定
    srvmanager_->PreDraw();

    // 描画コマンドを発行
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
#endif
}
