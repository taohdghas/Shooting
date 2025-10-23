#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

// ゲームシーンの初期化処理
void GameScene::Initialize() {

	// サウンド初期化
	Audio::GetInstance()->Initialize();

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");
	ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemybullet.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("platform/platform.obj");

	// パーティクルグループの生成
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateparticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateparticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);

	// レールカメラの初期化・設定
	railCamera = std::make_unique<RailCamera>();
	railCamera->Initialize();
	railCamera->SetPlayerOffset({ 0.0f,-1.5f,10.0f });
	railCamera->SetSpeed(0.1f);

	// メインカメラの初期化・登録
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0,0,-10 });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	// CameraManager::GetInstance()->AddCamera("Main", railCamera->GetCamera()); // レールカメラを使う場合
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// プラットフォームの初期化
	platform = std::make_unique<Platform>();
	platform->Initialize(Object3dBase::GetInstance());
	// railCamera->SetPlatform(platform.get());
	// railCamera->SetPlatformOffset({ 0.0f,-1.9f,10.0f });

	// 衝突管理クラスの初期化
	collisionManager = std::make_unique<CollisionManager>();

	// JsonManagerでレベルデータ読み込み
	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");

	// プレイヤー生成・初期化
	for (auto& playerData : levelData->players) {
		player = std::make_unique<Player>();
		player->Initialize(Object3dBase::GetInstance());
		Transform transform;
		transform.translate = playerData.translation;
		player->SetTranslate(transform.translate);
		// railCamera->SetPlayer(player.get());
	}

	// 敵生成・初期化
	for (auto& enemyData : levelData->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(Object3dBase::GetInstance());
		newEnemy->SetTranslate(enemyData.translation);
		newEnemy->SetPlayer(player.get());
		enemies.push_back(std::move(newEnemy));
	}

	// フェードの初期化・開始
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);

	// 最初の1フレーム入力を無視
	Input::GetInstance()->ClearInput();
}

// ゲームシーンの終了処理
void GameScene::Finalize() {
	// パーティクルグループの開放
	ParticleManager::GetInstance()->Clear();
	// カメラマネージャの終了処理
	CameraManager::GetInstance()->Finalize();
	// サウンドの終了処理
	Audio::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void GameScene::Update() {
	// カメラの更新
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	// レールカメラの更新
	railCamera->Update();

	// 敵ごとの衝突判定・デスパーティクル処理
	for (auto& enemy : enemies) {
		collisionManager->CheckPECollisions(player.get(), enemy.get());

		if (enemy->IsDeathParticle()) {
			auto emitter = std::make_unique<ParticleEmitter>();
			emitter->Initialize("particle3");
			emitter->SetPosition(enemy->GetPosition());
			emitter->Emit();
			particleEmitter.push_back(std::move(emitter));

			// デスパーティクルフラグをリセット
			enemy->SetisDeathParticle(false);
		}
	}

	// プレイヤーの更新
	player->Update();
	// 敵の更新
	for (auto& enemy : enemies) {
		enemy->Update();
	}
	// Skyboxの更新
	skybox->Update();
	// プラットフォームの更新
	platform->Update();
	
	// パーティクルの更新
	ParticleManager::GetInstance()->Update();
	for (auto& particle : particleEmitter) {
		particle->Update();
	}

	// プレイヤー死亡時はタイトルシーンへ遷移
	if (player->IsDead()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	// Cキーでクリアシーンへ遷移
	if (Input::GetInstance()->PushKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	// フェードの更新
	fade->Update();

	// デバッグ表示
	Debug();
}

// 描画処理
void GameScene::Draw() {
	// 3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	// プレイヤーの描画
	player->Draw();
	// 敵の描画
	for (auto& enemy : enemies) {
		enemy->Draw();
	}
	// 天球（Skybox）の描画
	skybox->Draw();
	// プラットフォームの描画
	platform->Draw();
	// パーティクルの描画
	ParticleManager::GetInstance()->Draw();
	// 共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
	// レティクル描画
	player->ReticleDraw();
	// フェード描画
	fade->Draw();
}

// デバッグ表示（ImGuiによるパラメータ調整・状態表示）
void GameScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	// カメラのパラメータ調整
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	// プレイヤーのデバッグ表示
	player->Debug();
	// 敵のデバッグ表示
	for (int i = 0; i < enemies.size(); ++i) {
		enemies[i]->Debug(i);
	}
	// プラットフォームのデバッグ表示
	platform->Debug();
	// Skyboxのデバッグ表示
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