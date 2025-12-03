#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include "MyMath.h"

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
	ModelManager::GetInstance()->LoadModel("boss/boss.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	ModelManager::GetInstance()->LoadModel("platform/platform.obj");

	// パーティクルグループの生成
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateparticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateparticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);
	ParticleManager::GetInstance()->CreateparticleGroup("particle6", "resources/circle.png", ParticleType::Smoke);
	ParticleManager::GetInstance()->CreateparticleGroup("playerMove", "resources/circle.png", ParticleType::PlayerMove);
	ParticleManager::GetInstance()->CreateparticleGroup("enemyDamage", "resources/enemydamage.png", ParticleType::EnemyDamage);

	// レールカメラの初期化・設定
	railCamera = std::make_unique<RailCamera>();
	railCamera->Initialize();
	railCamera->SetPlayerOffset({ 0.0f,-1.5f,10.0f });
	railCamera->SetPlatformOffset({ 0.0f,-1.9f,10.0f });
	railCamera->SetSpeed(0.2f);
	railCamera->EnableFollow(false);

	// メインカメラの初期化・登録
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0,0,-10 });
	camera->SetRotate({ 0, 0, 0 });
	CameraManager::GetInstance()->AddCamera("Start", camera.get());
    CameraManager::GetInstance()->AddCamera("Main", railCamera->GetCamera()); 
	CameraManager::GetInstance()->SetActiveCamera("Start");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//初期位置・回転を保存（StartCameraの値を保存）
	cameraStartPos = camera->GetTranslate();
	cameraStartRot = camera->GetRotate();

	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// プラットフォームの初期化
	platform = std::make_unique<Platform>();
	platform->Initialize(Object3dBase::GetInstance());
	railCamera->SetPlatform(platform.get());

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
		railCamera->SetPlayer(player.get());
	}

	// 敵生成・初期化
	for (auto& enemyData : levelData->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(Object3dBase::GetInstance());
		newEnemy->SetTranslate(enemyData.translation);
		newEnemy->SetPlayer(player.get());
		enemies.push_back(std::move(newEnemy));
	}

	//ボス(stage1)生成・初期化
	for (auto& bossData : levelData->bosses) {
		boss1 = std::make_unique<Boss1>();
		boss1->Initialize(Object3dBase::GetInstance());
		boss1->SetTranslate(bossData.translation);
		boss1->SetPlayer(player.get());
	}

	// UIの初期化
	ui = std::make_unique<Ui>();
	ui->Initialize();
	ui->SetPlayer(player.get());

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

	StartAnimation();

	if(!isStartAnimation && !isReturning) {
		//プレイヤー奥移動
		Vector3 playerPos = player->GetTranslate();
		playerPos.z += 0.2f; 
		player->SetTranslate(playerPos);

		//プラットフォーム奥移動
		Vector3 platPos = platform->GetTranslate();
		platPos.z += 0.2f; 
		platform->SetTranslate(platPos);
	}

	// レールカメラの更新
	railCamera->Update();

	// 敵ごとの衝突判定・デスパーティクル処理
	for (auto& enemy : enemies) {
		collisionManager->CheckPECollisions(player.get(), enemy.get());

		if (enemy->IsDeathParticle()) {
			auto emitter = std::make_unique<ParticleEmitter>();
			emitter->Initialize("particle2");
			emitter->SetPosition(enemy->GetPosition());
			emitter->Emit();
			particleEmitter.push_back(std::move(emitter));

			// デスパーティクルフラグをリセット
			enemy->SetIsDeathParticle(false);
		}
	}

	// プレイヤーの更新
	player->Update();
	// 敵の更新
	for (auto& enemy : enemies) {
		enemy->Update();
	}
	//ボス1の更新
	if (boss1) {
		boss1->Update();
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

	if (player->IsDead()) {
		isToGameOver = true;
	}

	if (Input::GetInstance()->PushKey(DIK_R)) {
		isToGameOver = true;
	}

	// z座標によるゲームクリア判定
	if(!isToGameOver){
//	if (!isToGameOver && player->GetTranslate().z > 250.0f) {
		ToGameClear();
	} else if (isToGameOver) {
		ToGameOver();
	}

	//UIの更新
	ui->Update();

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
	//ボス1の描画
	if (boss1) {
		boss1->Draw();
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
	// UIの描画
	ui->Draw();
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

		railCamera->Debug();
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
	//UIのデバッグ表示
	ui->Debug();
	ImGui::End();
#endif
}
//スタート演出
void GameScene::StartAnimation() {
	Camera* cam = camera.get();

	//スタート演出中
	if (isStartAnimation) {
		cameraRotateTimer += DeltaTime;
		float t = std::min(cameraRotateTimer / totalRotationTime, 1.0f);

		//イージング
		float easedT = static_cast<float>(easeInOutQuad(t));

		//回転角度
		float angle = easedT * oneRotation * rotationSpeed;

		// カメラ位置を円軌道で回す
		Vector3 camPos;
		camPos.x = cameraStartPos.x * std::cos(angle) - cameraStartPos.z * std::sin(angle);
		camPos.y = cameraStartPos.y;
		camPos.z = cameraStartPos.x * std::sin(angle) + cameraStartPos.z * std::cos(angle);
		cam->SetTranslate(camPos);

		Vector3 dir = player->GetTranslate() - camPos;
		float yaw = std::atan2(dir.x, dir.z);
		float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));
		cam->SetRotate({ pitch, yaw, 0 });

		//演出狩猟
		if (t >= 1.0f) {
			isStartAnimation = false;
			isReturning = true;
			cameraRotateTimer = 0.0f;
		}
		return;
	}

	//回転を初期角度へ戻すフェーズ
	if (isReturning) {
		cameraRotateTimer += DeltaTime;
		float t = std::min(cameraRotateTimer / 1.0f, 1.0f); 
		float easedT = static_cast<float>(easeInOutQuad(t));

		//現在の回転から0に戻す
		Vector3 curRot = cam->GetRotate();
		Vector3 targetRot = { 0,0,0 };
		Vector3 newRot;
		newRot.x = curRot.x + (targetRot.x - curRot.x) * easedT;
		newRot.y = curRot.y + (targetRot.y - curRot.y) * easedT;
		newRot.z = 0;
		cam->SetRotate(newRot);

		// RailCameraに切り替え
		if (t >= 1.0f) {
			isReturning = false;
			cameraRotateTimer = 0.0f;

			// RailCamera を有効化
			CameraManager::GetInstance()->SetActiveCamera("Main");
			railCamera->EnableFollow(true);

			// RailCamera 内部で追従初期化
			if (player) {
				Vector3 startCamPos = player->GetTranslate() + railCamera->GetPlayerOffset();
				railCamera->GetCamera()->SetTranslate(startCamPos);
			}
		}
	}
}

//ゲームクリアへ
void GameScene::ToGameClear() {
	//FadeInが終わっているなら状態をNoneに
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//フェードアウト開始
	if (fade->GetState() == Fade::State::None &&Input::GetInstance()->PushKey(DIK_C)) {
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
	//撃破演出開始
	if (!isDeathMotionStarted) {
		isDeathMotionStarted = true;
		deathTimer = 0.0f;
		deathRotationSpeed = 5.0f;

		//回転軸をランダムに
		deathRotationAxis.x = randomDist(randomEngine);
		deathRotationAxis.y = randomDist(randomEngine);
		deathRotationAxis.z = randomDist(randomEngine);
		Math::Normalize(deathRotationAxis);

		//飛び上がる初速度をランダムに生成
		float minY = 0.2f;
		float maxY = 0.3f;
		float velocityY = minY + (maxY - minY) * ((randomDist(randomEngine) + 1.0f) / 2.0f);

		//X/Z方向のランダム成分
		float velocityXZ = 0.05f;
		deathVelocity = {
			randomDist(randomEngine) * velocityXZ,
			velocityY,
			randomDist(randomEngine) * velocityXZ
		};
	}

	//撃破演出中
	if (isDeathMotionStarted) {
		deathTimer += DeltaTime;

		//回転
		Vector3 rot = player->GetRotate();
		rot.x += deathRotationAxis.x * deathRotationSpeed;
		rot.y += deathRotationAxis.y * deathRotationSpeed;
		rot.z += deathRotationAxis.z * deathRotationSpeed;
		player->SetRotate(rot);

		//落下
		deathVelocity.y -= gravity;
		Vector3 pos = player->GetTranslate();
		pos.x += deathVelocity.x;
		pos.y += deathVelocity.y;
		pos.z += deathVelocity.z;
		player->SetTranslate(pos);

		//地面寄りしたならフェードアウト開始
		if (pos.y < -3.0f && fade->GetState() == Fade::State::None) {
			fade->FadeStart(Fade::State::FadeOut, 0.5f);
		}
	}
	//フェードアウト終了後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("OVER");
	}
}