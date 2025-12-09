#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "CameraManager.h"
#include "ModelManager.h"

// ゲームクリアシーンの初期化処理
void GameClearScene::Initialize() {
	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("gameclearobject/g.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/a.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/m.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/e.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/c.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/l.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/r.obj");
	ModelManager::GetInstance()->LoadModel("gameclearobject/pushspace.obj");
	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera_.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());
	//Skybox
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//ゲームクリアオブジェクト
	game_clear_object_ = std::make_unique<GameClearObject>();
	game_clear_object_->Initialize();
	//フェード
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->FadeStart(Fade::State::FadeIn, 0.5f);
}

// ゲームクリアシーンの終了処理
void GameClearScene::Finalize() {
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void GameClearScene::Update() {
	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	//Skybox
	skybox_->Update();
	//ゲームクリアオブジェクト
	game_clear_object_->Update();
	//フェード
	fade_->Update();
	//シーン遷移
	SceneChange();
	//デバック
	Debug();
}

// ゲームクリアシーンの描画処理
void GameClearScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	//Skybox
	skybox_->Draw();
	//ゲームクリアオブジェクト
	game_clear_object_->Draw();
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
	//フェード
	fade_->Draw();
}
//デバック
void GameClearScene::Debug() {
	//ゲームクリアオブジェクト
	game_clear_object_->Debug();
}
//シーン遷移
void GameClearScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}
	//タイトルへのフェードアウト開始
	if (fade_->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_SPACE)) {
		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト後シーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}