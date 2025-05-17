#include "MyGame.h"
#include "SceneFactory.h"

//初期化
void MyGame::Initialize() {

	//基底クラスの初期化
	Framework::Initialize();

	//シーンファクトリーを生成してマネージャにセット
	sceneFactory = std::make_unique<SceneFactory>();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory.get());
    //シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");

}

//終了
void MyGame::Finalize() {
	//基底クラスの終了
	Framework::Finalize();
}

//毎フレーム更新
void MyGame::Update() {
	
	//ImGui開始
	imguimanager_->Begin();

	//基底クラスの更新
	Framework::Update();

	//ImGui終了
	imguimanager_->End();
}

//描画
void MyGame::Draw() {

    //RenderTexture描画準備
    directxBase_->PreDrawRenderTexture();

    SrvManager::GetInstance()->PreDraw();
    SceneManager::GetInstance()->Draw();

    //RenderTextureをSRV用に切り替え
    directxBase_->TransitionRenderTextureToSRV();

    //SwapChain描画準備
    directxBase_->PreDraw();

    //swapchainに描画
    directxBase_->DrawRenderTextureToScreen();

    //ImGui描画
    imguimanager_->Draw();


    //描画後処理
    directxBase_->PostDraw();

}
