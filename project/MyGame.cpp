#include "MyGame.h"

//初期化
void MyGame::Initialize() {

	//基底クラスの初期化
	Framework::Initialize();

	//タイトルシーンの生成と初期化
	titlescene_ = new TitleScene();
	titlescene_->Initialize();

	/*
	//ゲームシーンの生成
	gamescene_ = new GameScene();
	//ゲームシーンの初期化
	gamescene_->Initialize();
	*/
}

//終了
void MyGame::Finalize() {
	//タイトルシーンの終了
	titlescene_->Finalize();

	//タイトルシーンの解放
	delete titlescene_;

	//ゲームシーンの終了
	//gamescene_->Finalize();

	//シーンの解放
	//delete gamescene_;

	//基底クラスの終了
	Framework::Finalize();
}

//毎フレーム更新
void MyGame::Update() {
	
	//基底クラスの更新
	Framework::Update();

	//タイトルシーンの更新
	titlescene_->Update();

	//ゲームシーンの更新
	//gamescene_->Update();

	//ImGui開始
	imguimanager->Begin();

	//ImGui終了
	imguimanager->End();
}

//描画
void MyGame::Draw() {
	//描画前処理
	directxBase->PreDraw();

	srvManager->PreDraw();

	//タイトルシーン描画
	titlescene_->Draw();

	//ゲームシーン描画
	//gamescene_->Draw();

	//ImGui描画
	imguimanager->Draw();

	//描画後処理
	directxBase->PostDraw();
}
