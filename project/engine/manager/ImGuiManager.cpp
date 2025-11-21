#include "ImGuiManager.h"

ImGuiManager* ImGuiManager::instance_ = nullptr;

// シングルトンインスタンス取得
ImGuiManager* ImGuiManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new ImGuiManager;
    }
    return instance_;
}

// 終了処理
void ImGuiManager::Finalize() {
#ifdef USE_IMGUI
    // ImGuiのDirectX12とWin32用をシャットダウン
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif
    delete instance_;
    instance_ = nullptr;
}

// 初期化
void ImGuiManager::Initialize([[maybe_unused]] WindowsApi* windows_api, DirectXBase* directx_base, [[maybe_unused]] SrvManager* srv_manager) {
#ifdef USE_IMGUI
    windows_api_ = windows_api;
    directx_base_ = directx_base;
    srv_manager_ = srv_manager;

    // SRV用のインデックスを取得
    index_ = srv_manager_->Allocate();

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
    ImGui_ImplWin32_Init(windows_api_->GetHwnd());

    // DirectX12用初期化
    ImGui_ImplDX12_Init(
        directx_base_->GetDevice().Get(),
        static_cast<int>(directx_base_->GetSwapChainResourcesNum()),
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
        srv_manager_->GetDescriptorHeap(),
        srv_manager_->GetCpuDescriptorHandle(index_),
        srv_manager_->GetGpuDescriptorHandle(index_)
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
    ID3D12GraphicsCommandList* command_list = directx_base_->GetCommandList().Get();

    // デスクリプタヒープを設定
    srv_manager_->PreDraw();

    // 描画コマンドを発行
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), command_list);
#endif
}
