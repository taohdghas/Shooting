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
	SceneManager::GetInstance()->ChangeScene("GAME");
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

#ifdef USE_IMGUI
	//RenderTextureの切り替え
	if (ImGui::Begin("MyGame SetUp")) {
		ImGui::Checkbox("Use RenderTexture", & useRenderTexture_);
	}
	ImGui::End();
#endif

	//ImGui終了
	imguimanager_->End();
}

//描画
void MyGame::Draw() {

	//RenderTexture使用時
	if (useRenderTexture_) {

		//RenderTexture描画準備
		DirectXBase::GetInstance()->PreDrawRenderTexture();
		//Srv描画準備
		SrvManager::GetInstance()->PreDraw();
		//シーン描画
		SceneManager::GetInstance()->Draw();
		//RenderTextureをSRV用に切り替え
		DirectXBase::GetInstance()->TransitionRenderTextureToSRV();
		//SwapChain描画準備
		DirectXBase::GetInstance()->PreDraw();
		//swapchainに描画
		DirectXBase::GetInstance()->DrawRenderTextureToScreen();
		//ImGui描画
		imguimanager_->Draw();
		//描画後処理
		DirectXBase::GetInstance()->PostDraw();
	}
	//不使用
	else {
		//SwapChain描画準備
		DirectXBase::GetInstance()->PreDraw();
		//Srv描画準備
		SrvManager::GetInstance()->PreDraw();
		//シーン描画
		SceneManager::GetInstance()->Draw();
		//ImGui描画
		imguimanager_->Draw();
		//描画後処理
		DirectXBase::GetInstance()->PostDraw();
	}
}
