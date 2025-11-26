#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "CameraManager.h"
#include "ModelManager.h"
#include "ParticleManager.h"

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
	//パーティクルマネージャ
	ParticleManager::GetInstance()->CreateparticleGroup("confetti", "resources/confetti.png", ParticleType::Confetti);
	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());
	//Skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//ゲームクリアオブジェクト
	gameClearObject = std::make_unique<GameClearObject>();
	gameClearObject->Initialize();
	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);

	auto confettiEmitter = std::make_unique<ParticleEmitter>();
	confettiEmitter->Initialize("confetti");
	particleEmitter_.push_back(std::move(confettiEmitter));
}

// ゲームクリアシーンの終了処理
void GameClearScene::Finalize() {
	// パーティクルグループの開放
	ParticleManager::GetInstance()->Clear();
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void GameClearScene::Update() {
	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	//Skybox
	skybox->Update();
	//ゲームクリアオブジェクト
	gameClearObject->Update();
	//フェード
	fade->Update();
	// 紙吹雪エミッターの更新
		for (auto& emitter : particleEmitter_) {
			Vector3 pos{ 0.0f, 3.0f, 0.0f }; 
			emitter->SetPosition(pos);
			emitter->Update();
		}
	// パーティクルの更新
	ParticleManager::GetInstance()->Update();

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
	//ゲームクリアオブジェクト
	gameClearObject->Draw();
	//パーティクル描画
	ParticleManager::GetInstance()->Draw();
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}
//デバック
void GameClearScene::Debug() {
	//ゲームクリアオブジェクト
	gameClearObject->Debug();
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