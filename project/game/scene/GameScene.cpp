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


	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0,0,-11 });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//初期位置・回転を保存
	cameraStartPos = camera->GetTranslate();
	cameraStartRot = camera->GetRotate();
	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// プラットフォームの初期化
	platform = std::make_unique<Platform>();
	platform->Initialize(Object3dBase::GetInstance());

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
		player->SetPlatform(platform.get());
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
	FollowCamera();

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
	
	//プレイヤー
	player->Update(isStartAnimation, isReturning);
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

	//プラットフォームの更新
	platform->Update(isStartAnimation, isReturning);
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
	//敵
	if (!isStartAnimation) {
		for (auto& enemy : enemies) {
			enemy->Draw();
		}
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
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
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

	if (!isStartAnimation && Input::GetInstance()->PushKey(DIK_H)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

	// Update内
	if (isStartAnimation) {
		cameraRotateTimer += 1.0f / 60.0f;
		float t = std::min(cameraRotateTimer / totalRotationTime, 1.0f);

		//イージング適用
		float easedT = static_cast<float>(easeInOutQuad(t));

		//イージングを反映後角度
		float angle = easedT * oneRotation * rotationSpeed;

		//初期カメラ位置
		Vector3 camPos;
		camPos.x = cameraStartPos.x * std::cos(angle) - cameraStartPos.z * std::sin(angle);
		camPos.y = cameraStartPos.y; // 高さはそのまま
		camPos.z = cameraStartPos.x * std::sin(angle) + cameraStartPos.z * std::cos(angle);

		Camera* cam = CameraManager::GetInstance()->GetActiveCamera();
		cam->SetTranslate(camPos);

		//プレイヤー方向を向く
		Vector3 dir = player->GetTranslate() - camPos;
		float yaw = std::atan2(dir.x, dir.z);
		float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));

		cam->SetRotate({ cameraStartRot.x + pitch, cameraStartRot.y + yaw, cameraStartRot.z });
		if (t >= 1.0f) {
			isStartAnimation = false;
			isReturning = true;
			cameraRotateTimer = 0.0f;
		}
	} else if (isReturning) {
		//回転終了後角度を初期値に戻す
		Camera* cam = CameraManager::GetInstance()->GetActiveCamera();
		Vector3 currentRot = cam->GetRotate();
		Vector3 targetRot = { 0.0f, 0.0f, 0.0f };

		//イージングタイマーを更新
		cameraRotateTimer += 1.0f / 60.0f;
		float t = std::min(cameraRotateTimer / 2.0f, 1.0f);
		float easedT = static_cast<float>(easeInOutQuad(t));

		Vector3 newRot;
		newRot.x = currentRot.x + (targetRot.x - currentRot.x) * easedT * 0.1f;
		newRot.y = currentRot.y + (targetRot.y - currentRot.y) * easedT * 0.1f;
		newRot.z = currentRot.z + (targetRot.z - currentRot.z) * easedT * 0.1f;

		cam->SetRotate(newRot);
		//収束終了
		if (t >= 1.0f) {
			isReturning = false;
			cameraRotateTimer = 0.0f;
		}
	}
}

void GameScene::FollowCamera() {
	Camera* cam = CameraManager::GetInstance()->GetActiveCamera();

	// スタート演出中は追従しない
	if (isStartAnimation || isReturning) {
		isFollowingInitialized = false;
		return;
	}

	// 追従開始時にZオフセットを記録
	if (!isFollowingInitialized) {
		zOffset = cam->GetTranslate().z - platform->GetTranslate().z;
		isFollowingInitialized = true;
	}

	// Z方向追従
	Vector3 pos = cam->GetTranslate();
	float followSpeed = 0.1f;
	float targetZ = platform->GetTranslate().z + zOffset;
	pos.z += (targetZ - pos.z) * followSpeed;

	cam->SetTranslate(pos);
}
