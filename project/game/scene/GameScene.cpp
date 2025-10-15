#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"


//初期化
void GameScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");
	ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemybullet.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("platform/platform.obj");

	//パーティクル
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateparticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateparticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);

	//レールカメラ
	railCamera = std::make_unique<RailCamera>();
	railCamera->Initialize();
	railCamera->SetPlayerOffset({ 0.0f,-1.5f,10.0f });
	railCamera->SetSpeed(0.1f);
	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0,0,-10 });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	//CameraManager::GetInstance()->AddCamera("Main", railCamera->GetCamera());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//プラットフォーム
	platform = std::make_unique<Platform>();
	platform->Initialize(Object3dBase::GetInstance());
	//railCamera->SetPlatform(platform.get());
	//railCamera->SetPlatformOffset({ 0.0f,-1.9f,10.0f });

	//衝突マネージャー
	collisionManager = std::make_unique<CollisionManager>();

	//JsonManager
	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");

	for (auto& playerData : levelData->players) {
		player = std::make_unique<Player>();
		player->Initialize(Object3dBase::GetInstance());
		Transform transform;

		transform.translate = playerData.translation;

		player->SetTranslate(transform.translate);
		//railCamera->SetPlayer(player.get());
	}

	for (auto& enemyData : levelData->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(Object3dBase::GetInstance());
		newEnemy->SetTranslate(enemyData.translation);
		newEnemy->SetPlayer(player.get());
		enemies.push_back(std::move(newEnemy));
	}

	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);

	//最初の1フレーム入力を無視
	Input::GetInstance()->ClearInput();

}

//終了
void GameScene::Finalize() {
	//パーティクルグループの開放
	ParticleManager::GetInstance()->Clear();
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {
	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	//レールカメラ
	railCamera->Update();

	//衝突チェック
	for (auto& enemy : enemies) {
		collisionManager->CheckPECollisions(player.get(), enemy.get());

		if (enemy->IsDeathParticle()) {
			auto emitter = std::make_unique<ParticleEmitter>();
			emitter->Initialize("particle3");
			emitter->SetPosition(enemy->GetPosition());
			emitter->Emit();
			particleEmitter.push_back(std::move(emitter));

			//フラグをリセット
			enemy->SetisDeathParticle(false);
		}
	}
	//プレイヤー
	player->Update();
	//敵
	for (auto& enemy : enemies) {
		enemy->Update();
	}
	//skybox
	skybox->Update();
	//プラットフォーム
	platform->Update();

	//パーティクル
	ParticleManager::GetInstance()->Update();
	for (auto& particle : particleEmitter) {
		particle->Update();
	}

	//クリアシーンへ
	ToGameClear();

	//プレイヤーが撃破されたらゲームオーバーへ
	if (player->IsDead()) {
		ToGameOver();
	}

	fade->Update();

	//デバック
	Debug();
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	//プレイヤー
	player->Draw();
	//敵
	for (auto& enemy : enemies) {
		enemy->Draw();
	}
	//天球
	skybox->Draw();
	//プラットフォーム
	platform->Draw();
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();

	//パーティクル
	ParticleManager::GetInstance()->Draw();

}

//デバック
void GameScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	//カメラ
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	//プレイヤーDebug
	player->Debug();
	//敵Debug
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i]->Debug(i);
	}
	//プラットフォームDebug
	platform->Debug();
	//skyboxDebug
	Transform& trans = skybox->GetTransform();
	if (ImGui::TreeNode("SkyBox")) {
		ImGui::DragFloat3("Scale", &trans.scale.x, 0.01f, 0.01f, 5000.0f);
		ImGui::DragFloat3("Rotate", &trans.rotate.x, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Translate", &trans.translate.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::TreePop();
	}
	ImGui::End();
#endif
}
//ゲームクリアへ
void GameScene::ToGameClear() {
	//FadeInが終わっているなら状態をNoneに
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//フェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->PushKey(DIK_C)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト終了後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
}
//ゲームオーバーへ
void GameScene::ToGameOver() {
	//FadeInが終わっているなら状態をNoneに
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//フェードアウト開始
	if (fade->GetState() == Fade::State::None) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト終了後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("OVER");
	}
}