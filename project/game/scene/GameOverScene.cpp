#include "GameOverScene.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "Input.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ModelManager.h"

//初期化
void GameOverScene::Initialize() {

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("gameoverobject/retry.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/g.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/a.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/m.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/e.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/o.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/v.obj");
	ModelManager::GetInstance()->LoadModel("gameoverobject/r.obj");

	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera_.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//フェード
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->FadeStart(Fade::State::FadeIn, 0.5f);
	//ゲームオーバーオブジェクト
	game_over_object_ = std::make_unique<GameOverObject>();
	game_over_object_->Initialize();
	//Skybox
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
}
//終了
void GameOverScene::Finalize() {
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
}
//更新
void GameOverScene::Update() {

	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	//Skybox
	skybox_->Update();

	//ゲームオーバーオブジェクト
	game_over_object_->Update();
	//フェード
	fade_->Update();
	//シーン遷移
	SceneChange();
	//デバック
	Debug();
}
//描画
void GameOverScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	//Skybox
	skybox_->Draw();

	//ゲームオーバーオブジェクト
	game_over_object_->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade_->Draw();
}
//デバック
void GameOverScene::Debug() {
	//ゲームオーバーオブジェクト
	game_over_object_->Debug();

}
//シーン遷移
void GameOverScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}
	//タイトルへのフェードアウト開始
	if (fade_->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_SPACE)) {
		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
		is_to_title_ = true;
	}
	//ゲームシーンへのフェードアウト(リトライ)
	if (fade_->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_R)) {
		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
		is_to_game_ = true;
	}
	//タイトルシーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()&&is_to_title_) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//ゲームシーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished() && is_to_game_) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}