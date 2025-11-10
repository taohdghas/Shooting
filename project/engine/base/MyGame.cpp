#include "MyGame.h"
#include "SceneFactory.h"

// 初期化
void MyGame::Initialize() {

    // 基底クラスの初期化
    Framework::Initialize();

    // シーンファクトリーを生成し、シーンマネージャにセット
    sceneFactory = std::make_unique<SceneFactory>();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory.get());

    // 最初のシーンをTITLEに設定
    SceneManager::GetInstance()->ChangeScene("GAME");
}

// 終了処理
void MyGame::Finalize() {
    // 基底クラスの終了
    Framework::Finalize();
}

// 毎フレーム更新
void MyGame::Update() {

    // ImGui描画開始
    imguimanager_->Begin();

    // 基底クラスの更新（入力処理やシーン更新など）
    Framework::Update();

#ifdef USE_IMGUI
    // ImGuiでRenderTextureの使用設定を切り替え
    if (ImGui::Begin("MyGame SetUp")) {
        ImGui::Checkbox("Use RenderTexture", &useRenderTexture_);
    }
    ImGui::End();
#endif

    // ImGui描画終了
    imguimanager_->End();
}

// 描画処理
void MyGame::Draw() {

    if (useRenderTexture_) {
        // RenderTextureに描画する場合のフロー

        // RenderTexture描画開始
        DirectXBase::GetInstance()->PreDrawRenderTexture();

        // シェーダリソース準備
        SrvManager::GetInstance()->PreDraw();

        // 現在のシーンを描画
        SceneManager::GetInstance()->Draw();

        // RenderTextureをSRV用に切り替え
        DirectXBase::GetInstance()->TransitionRenderTextureToSRV();

        // SwapChain描画準備
        DirectXBase::GetInstance()->PreDraw();

        // RenderTextureの内容を画面に描画
        DirectXBase::GetInstance()->DrawRenderTextureToScreen();

        // ImGui描画
        imguimanager_->Draw();

        // 描画後処理（バッファ切り替えなど）
        DirectXBase::GetInstance()->PostDraw();
    } else {
        // 通常SwapChainに直接描画する場合のフロー

        DirectXBase::GetInstance()->PreDraw();
        SrvManager::GetInstance()->PreDraw();
        SceneManager::GetInstance()->Draw();
        imguimanager_->Draw();
        DirectXBase::GetInstance()->PostDraw();
    }
}
