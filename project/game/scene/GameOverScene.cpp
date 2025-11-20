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
	ModelManager::GetInstance()->LoadModel("gameover.obj");
	ModelManager::GetInstance()->LoadModel("retry.obj");
	ModelManager::GetInstance()->LoadModel("g.obj");
	ModelManager::GetInstance()->LoadModel("a.obj");
	ModelManager::GetInstance()->LoadModel("m.obj");
	ModelManager::GetInstance()->LoadModel("e.obj");
	ModelManager::GetInstance()->LoadModel("o.obj");
	ModelManager::GetInstance()->LoadModel("v.obj");
	ModelManager::GetInstance()->LoadModel("r.obj");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);
	//ゲームオーバーオブジェクト
	gameOverObject = std::make_unique<GameOverObject>();
	gameOverObject->Initialize();
	//Skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
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
	skybox->Update();

	//ゲームオーバーオブジェクト
	gameOverObject->Update();
	//フェード
	fade->Update();
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
	skybox->Draw();

	//ゲームオーバーオブジェクト
	gameOverObject->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}
//デバック
void GameOverScene::Debug() {
	//ゲームオーバーオブジェクト
	gameOverObject->Debug();

}
//シーン遷移
void GameOverScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//タイトルへのフェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
		isToTitle = true;
	}
	//ゲームシーンへのフェードアウト(リトライ)
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_R)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
		isToGame = true;
	}
	//タイトルシーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()&&isToTitle) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	//ゲームシーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished() && isToGame) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}