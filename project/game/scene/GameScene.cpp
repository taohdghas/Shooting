#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// ゲームシーンの初期化処理ad
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
	ParticleManager::GetInstance()->CreateParticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateParticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateParticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateParticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateParticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);

	// レールカメラの初期化・設定
	rail_camera_ = std::make_unique<RailCamera>();
	rail_camera_->Initialize();
	rail_camera_->SetPlayerOffset({ 0.0f,-1.5f,10.0f });
	rail_camera_->SetSpeed(0.1f);

	// メインカメラの初期化・登録
	camera_ = std::make_unique<Camera>();
	camera_->SetTranslate({ 0,0,-10 });
	CameraManager::GetInstance()->AddCamera("Main", camera_.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	// Skyboxの初期化
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// プラットフォームの初期化
	platform_ = std::make_unique<Platform>();
	platform_->Initialize(Object3dBase::GetInstance());
	// railCamera->SetPlatform(platform.get());
	// // railCamera->SetPlatformOffset({ 0.0f,-1.9f,10.0f });

	// 衝突管理クラスの初期化
	collision_manager_ = std::make_unique<CollisionManager>();

	// JsonManagerでレベルデータ読み込み
	json_manager_ = std::make_unique<JsonManager>();
	level_data_ = json_manager_->LoadJsonFile("untitled");

	// プレイヤー生成・初期化
	for (auto& playerData : level_data_->players) {
		player_ = std::make_unique<Player>();
		player_->Initialize(Object3dBase::GetInstance());
		Transform transform;
		transform.translate = playerData.translation;
		player_->SetTranslate(transform.translate);
	}

	// 敵生成・初期化
	for (auto& enemyData : level_data_->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(Object3dBase::GetInstance());
		newEnemy->SetTranslate(enemyData.translation);
		newEnemy->SetPlayer(player_.get());

		// Rails がある場合セット
		if (!enemyData.rails.empty()) {
			// ここでは最初のRailsのみ使用
			const auto& rail = enemyData.rails[0];
			newEnemy->SetRail(rail.controlPoints, rail.closed);
		}


		enemies_.push_back(std::move(newEnemy));
	}

	// フェードの初期化・開始
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->FadeStart(Fade::State::FadeIn, 0.5f);

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
	rail_camera_->Update();

	// 敵ごとの衝突判定・デスパーティクル処理
	for (auto& enemy : enemies_) {
		collision_manager_->CheckPlayerEnemyCollisions(player_.get(), enemy.get());

		if (enemy->IsDeathParticle()) {
			auto emitter = std::make_unique<ParticleEmitter>();
			emitter->Initialize("particle3");
			emitter->SetPosition(enemy->GetPosition());
			emitter->Emit();
			particle_emitters_.push_back(std::move(emitter));

			// デスパーティクルフラグをリセット
			enemy->SetIsDeathParticle(false);
		}
	}

	// プレイヤーの更新
	player_->Update();
	// 敵の更新
	for (auto& enemy : enemies_) {
		enemy->Update();
	}
	// Skyboxの更新
	skybox_->Update();
	// プラットフォームの更新
	platform_->Update();

	// パーティクルの更新
	ParticleManager::GetInstance()->Update();
	for (auto& particle : particle_emitters_) {
		particle->Update();
	}

	if (player_->IsDead()) {
		is_to_game_over_ = true;
	}

	if (Input::GetInstance()->IsKeyPressed(DIK_R)) {
		is_to_game_over_ = true;
	}

	if (!is_to_game_over_) {
		ToGameClear();
	} else {
		ToGameOver();
	}

	fade_->Update();

	// デバッグ表示
	Debug();
}

// 描画処理
void GameScene::Draw() {
	// 3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	// プレイヤーの描画
	player_->Draw();
	// 敵の描画
	for (auto& enemy : enemies_) {
		enemy->Draw();
	}
	// 天球（Skybox）の描画
	skybox_->Draw();
	// プラットフォームの描画
	platform_->Draw();
	// パーティクルの描画
	ParticleManager::GetInstance()->Draw();

	// 共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
	// レティクル描画
	player_->ReticleDraw();
	// フェード描画
	fade_->Draw();

	//パーティクル
	ParticleManager::GetInstance()->Draw();
	//パーティクル（重複呼び出しは仕様に合わせ保持）
	ParticleManager::GetInstance()->Draw();
}

// デバッグ表示（ImGuiによるパラメータ調整・状態表示）
void GameScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	// カメラのパラメータ調整
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera_->GetTranslate();
		Vector3 cameraRot = camera_->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera_->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera_->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	// プレイヤーのデバッグ表示
	player_->Debug();
	// 敵のデバッグ表示
	for (int i = 0; i < enemies_.size(); ++i) {
		enemies_[i]->Debug(i);
	}
	// プラットフォームのデバッグ表示
	platform_->Debug();
	// Skyboxのデバッグ表示
	Transform& trans = skybox_->GetTransform();
	if (ImGui::TreeNode("SkyBox")) {
		ImGui::DragFloat3("Scale", &trans.scale.x, 0.01f, 0.01f, 5000.0f);
		ImGui::DragFloat3("Rotate", &trans.rotate.x, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Translate", &trans.translate.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::TreePop();
	}
	ImGui::End();
#endif
}

// ゲームクリアへ
void GameScene::ToGameClear() {
	// FadeIn が終わっているなら状態を None に
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}
	// フェードアウト開始
	if (fade_->GetState() == Fade::State::None &&
		Input::GetInstance()->IsKeyPressed(DIK_C))
	{
		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	// フェードアウト終了後シーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}
}

// ゲームオーバーへ
void GameScene::ToGameOver() {
	// FadeIn が終わっているなら状態を None に
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}

	// 撃破演出開始
	if (!is_death_motion_started_) {
		is_death_motion_started_ = true;
		death_timer_ = 0.0f;
		death_rotation_speed_ = 5.0f;

		// 回転軸をランダムに
		death_rotation_axis_.x = random_dist_(random_engine_);
		death_rotation_axis_.y = random_dist_(random_engine_);
		death_rotation_axis_.z = random_dist_(random_engine_);
		Math::Normalize(death_rotation_axis_);

		// 飛び上がる初速度をランダムに生成
		float minY = 0.2f;
		float maxY = 0.3f;
		float velocityY =
			minY + (maxY - minY) * ((random_dist_(random_engine_) + 1.0f) / 2.0f);

		// X/Z 方向のランダム成分
		float velocityXZ = 0.05f;
		death_velocity_ = {
			random_dist_(random_engine_) * velocityXZ,
			velocityY,
			random_dist_(random_engine_) * velocityXZ
		};
	}

	// 撃破演出中
	if (is_death_motion_started_) {
		death_timer_ += kDeltaTime;

		// 回転
		Vector3 rot = player_->GetRotate();
		rot.x += death_rotation_axis_.x * death_rotation_speed_;
		rot.y += death_rotation_axis_.y * death_rotation_speed_;
		rot.z += death_rotation_axis_.z * death_rotation_speed_;
		player_->SetRotate(rot);

		// 落下
		death_velocity_.y -= gravity_;
		Vector3 pos = player_->GetTranslate();
		pos.x += death_velocity_.x;
		pos.y += death_velocity_.y;
		pos.z += death_velocity_.z;
		player_->SetTranslate(pos);

		// 地面より下ならフェードアウト開始
		if (pos.y < -3.0f && fade_->GetState() == Fade::State::None) {
			fade_->FadeStart(Fade::State::FadeOut, 0.5f);
		}
	}
	// フェードアウト終了後シーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("OVER");
	}
}
