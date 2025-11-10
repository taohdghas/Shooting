#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "CameraManager.h"
#include "ModelManager.h"

// ゲームクリアシーンの初期化処理
void GameClearScene::Initialize() {
	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());
	//Skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);
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
	skybox->Update();

	//フェード
	fade->Update();
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
	skybox->Draw();
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}
//デバック
void GameClearScene::Debug() {

}
//シーン遷移
void GameClearScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//タイトルへのフェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}