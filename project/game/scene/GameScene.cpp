#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// ゲームシーンの初期化処理ad
void GameScene::Initialize() {

	// サウンド初期化
	MyEngine::Audio::GetInstance()->Initialize();

	// モデルの読み込み
	MyEngine::ModelManager::GetInstance()->LoadModel("plane.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("axis.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("player/player.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("enemy/enemybullet.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("boss/boss.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("platform/platform.obj");

	// パーティクルグループの生成
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("particle", "resources/uvChecker.png", MyEngine::ParticleType::Normal);
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("particle2", "resources/circle2.png", MyEngine::ParticleType::Normal);
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("particle3", "resources/gradationLine.png", MyEngine::ParticleType::Ring);
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("particle4", "resources/gradationLine.png", MyEngine::ParticleType::Cylinder);
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("particle5", "resources/circle2.png", MyEngine::ParticleType::Explosive);
	MyEngine::ParticleManager::GetInstance()->CreateParticleGroup("platformsmoke", "resources/circle.png", MyEngine::ParticleType::Smoke);

	//カメラの初期化・設定
	camera_ = std::make_unique< MyEngine::Camera>();
	camera_->SetRotate({ 0.0f,0.0f,0.0f });
	camera_->SetTranslate({ 0,0,-11 });
	MyEngine::CameraManager::GetInstance()->AddCamera("Main", camera_.get());
	MyEngine::CameraManager::GetInstance()->SetActiveCamera("Main");
	MyEngine::Object3dBase::GetInstance()->SetDefaultCamera(MyEngine::CameraManager::GetInstance()->GetActiveCamera());

	//初期位置・回転を保存
	camera_start_pos_ = camera_->GetTranslate();
	camera_start_rot_ = camera_->GetRotate();

	// Skyboxの初期化
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// プラットフォームの初期化
	platform_ = std::make_unique<Platform>();
	platform_->Initialize(MyEngine::Object3dBase::GetInstance());

	// 衝突管理クラスの初期化
	collision_manager_ = std::make_unique<CollisionManager>();

	// JsonManagerでレベルデータ読み込み
	json_manager_ = std::make_unique< MyEngine::JsonManager>();
	level_data_ = json_manager_->LoadJsonFile("untitled");

	// プレイヤー生成・初期化
	for (auto& playerData : level_data_->players) {
		player_ = std::make_unique<Player>();
		player_->Initialize(MyEngine::Object3dBase::GetInstance());
		Transform transform;
		transform.translate = playerData.translation;
		player_->SetTranslate(transform.translate);
		player_->SetPlatform(platform_.get());
	}

	// 敵生成・初期化
	for (auto& enemyData : level_data_->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(MyEngine::Object3dBase::GetInstance());
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

	// ボス生成・初期化
	if (!level_data_->bosses.empty()) {

		//ボスは1体想定
		const auto& bossData = level_data_->bosses[0];

		boss_ = std::make_unique<Boss1>();
		boss_->Initialize(MyEngine::Object3dBase::GetInstance());

		//座標設定
		boss_->SetTranslate(bossData.translation);

		//プレイヤー参照を渡す
		boss_->SetPlayer(player_.get());
	}



	// フェードの初期化・開始
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->FadeStart(Fade::State::FadeIn, 0.5f);

	// UIの初期化
	ui_ = std::make_unique<Ui>();
	ui_->Initialize();
	ui_->SetPlayer(player_.get());

	// 最初の1フレーム入力を無視
	MyEngine::Input::GetInstance()->ClearInput();
}


// ゲームシーンの終了処理
void GameScene::Finalize() {
	// パーティクルグループの開放
	MyEngine::ParticleManager::GetInstance()->Clear();
	// カメラマネージャの終了処理
	MyEngine::CameraManager::GetInstance()->Finalize();
	// サウンドの終了処理
	MyEngine::Audio::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void GameScene::Update() {
	// カメラの更新
	MyEngine::CameraManager::GetInstance()->GetActiveCamera()->Update();

	// スタート演出
	StartAnimation();

	// 追従カメラ
	FollowCamera();

	// ゲームフェーズごとの処理
	if (game_phase_ == GamePhase::Stage) {
		// ボストリガー到達判定
		if (platform_->GetTranslate().z >= kBossTriggerZ) {
			game_phase_ = GamePhase::BossBattle;

			// スクロール停止
			platform_->Stop();

			// プレイヤー追従解除
			player_->SetFollowPlatform(false);

			// カメラ追従解除
			is_following_initialized_ = false;
		}
	}

	// 敵ごとの衝突判定・デスパーティクル処理
	for (auto& enemy : enemies_) {
		collision_manager_->CheckPlayerEnemyCollisions(player_.get(), enemy.get());

		if (enemy->IsDeathParticle()) {
			auto emitter = std::make_unique< MyEngine::ParticleEmitter>();
			emitter->Initialize("particle3");
			emitter->SetPosition(enemy->GetPosition());
			emitter->Emit();
			particle_emitters_.push_back(std::move(emitter));

			// デスパーティクルフラグをリセット
			enemy->SetIsDeathParticle(false);
		}
	}

	// プレイヤーの更新
	player_->Update(is_start_animation_, is_returning_);
	// 敵の更新
	if (!is_start_animation_ && !is_returning_) {
		for (auto& enemy : enemies_) {
			collision_manager_->CheckPlayerBpssCollisions(player_.get(), boss_.get());
			enemy->Update();
		}
	}
	// ボスの更新
	if (game_phase_ == GamePhase::BossBattle) {
		boss_->Update();
	}
	// Skyboxの更新
	skybox_->Update();
	// プラットフォームの更新
	platform_->Update(is_start_animation_, is_returning_);

	// パーティクルの更新
	MyEngine::ParticleManager::GetInstance()->Update();
	for (auto& particle : particle_emitters_) {
		particle->Update();
	}

	if (player_->IsDead()) {
		is_to_game_over_ = true;
	}

	if (MyEngine::Input::GetInstance()->IsKeyPressed(DIK_R)) {
		is_to_game_over_ = true;
	}

	if (!is_to_game_over_) {
		ToGameClear();
	} else {
		ToGameOver();
	}

	// フェードの更新
	fade_->Update();

	// UIの更新
	ui_->Update();

	// デバッグ表示
	Debug();
}

// 描画処理
void GameScene::Draw() {
	// 3Dオブジェクト描画準備
	MyEngine::Object3dBase::GetInstance()->DrawBaseSet();
	// プレイヤーの描画
	player_->Draw();
	// 敵の描画
	if (!is_start_animation_ && !is_returning_) {
		for (auto& enemy : enemies_) {
			enemy->Draw();
		}
	}
	if (game_phase_ == GamePhase::BossBattle) {
		// ボスの描画
		boss_->Draw();
	}
	// 天球（Skybox）の描画
	skybox_->Draw();
	// プラットフォームの描画
	platform_->Draw();
	// パーティクルの描画
	MyEngine::ParticleManager::GetInstance()->Draw();

	// 共通描画設定
	MyEngine::SpriteBase::GetInstance()->DrawBaseSet();
	// レティクル描画
	player_->ReticleDraw();
	// フェード描画
	fade_->Draw();
	// UI描画
	ui_->Draw();
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

//追従カメラ
void GameScene::FollowCamera() {
	MyEngine::Camera* cam = MyEngine::CameraManager::GetInstance()->GetActiveCamera();

	// スタート演出中は追従しない
	if (is_start_animation_ || is_returning_) {
		is_following_initialized_ = false;
		return;
	}
	// ボス戦中は追従しない
	if (game_phase_ != GamePhase::Stage) {
		is_following_initialized_ = false;
		return;
	}

	// 追従開始時にZオフセットを記録
	if (!is_following_initialized_) {
		z_offset_ = cam->GetTranslate().z - platform_->GetTranslate().z;
		is_following_initialized_ = true;
	}

	// Z方向追従
	Vector3 pos = cam->GetTranslate();
	float followSpeed = 0.1f;
	float targetZ = platform_->GetTranslate().z + z_offset_;
	pos.z += (targetZ - pos.z) * followSpeed;

	cam->SetTranslate(pos);
}

//スタート演出
void GameScene::StartAnimation() {
	if (!is_start_animation_ && MyEngine::Input::GetInstance()->IsKeyPressed(DIK_H)) {
		MyEngine::SceneManager::GetInstance()->ChangeScene("GAME");
	}

	// Update内
	if (is_start_animation_) {
		camera_rotate_timer_ += 1.0f / 60.0f;
		float t = std::min(camera_rotate_timer_ / totalRotationTime, 1.0f);

		//イージング適用
		float easedT = static_cast<float>(easeInOutQuad(t));

		//イージングを反映後角度
		float angle = easedT * oneRotation * rotationSpeed;

		//初期カメラ位置
		Vector3 camPos;
		camPos.x = camera_start_pos_.x * std::cos(angle) - camera_start_pos_.z * std::sin(angle);
		camPos.y = camera_start_pos_.y; // 高さはそのまま
		camPos.z = camera_start_pos_.x * std::sin(angle) + camera_start_pos_.z * std::cos(angle);

		MyEngine::Camera* cam = MyEngine::CameraManager::GetInstance()->GetActiveCamera();
		cam->SetTranslate(camPos);

		//プレイヤー方向を向く
		Vector3 dir = player_->GetTranslate() - camPos;
		float yaw = std::atan2(dir.x, dir.z);
		float pitch = -std::atan2(dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));

		cam->SetRotate({ camera_start_rot_.x + pitch, camera_start_rot_.y + yaw, camera_start_rot_.z });
		if (t >= 1.0f) {
			is_start_animation_ = false;
			is_returning_ = true;
			camera_rotate_timer_ = 0.0f;
		}
	} else if (is_returning_) {
		//回転終了後角度を初期値に戻す
		MyEngine::Camera* cam = MyEngine::CameraManager::GetInstance()->GetActiveCamera();
		Vector3 currentRot = cam->GetRotate();
		Vector3 targetRot = { 0.0f, 0.0f, 0.0f };

		//イージングタイマーを更新
		camera_rotate_timer_ += 1.0f / 60.0f;
		float t = std::min(camera_rotate_timer_ / 2.0f, 1.0f);
		float easedT = static_cast<float>(easeInOutQuad(t));

		Vector3 newRot;
		newRot.x = currentRot.x + (targetRot.x - currentRot.x) * easedT * 0.1f;
		newRot.y = currentRot.y + (targetRot.y - currentRot.y) * easedT * 0.1f;
		newRot.z = currentRot.z + (targetRot.z - currentRot.z) * easedT * 0.1f;

		cam->SetRotate(newRot);
		//収束終了
		if (t >= 1.0f) {
			is_returning_ = false;
			camera_rotate_timer_ = 0.0f;
		}
	}
}

// ゲームクリアへ
void GameScene::ToGameClear() {
	// FadeIn が終わっているなら状態を None に
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}
	// フェードアウト開始
	if (fade_->GetState() == Fade::State::None &&
		MyEngine::Input::GetInstance()->IsKeyPressed(DIK_C))
	{
		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	// フェードアウト終了後シーン移行
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()) {
		MyEngine::SceneManager::GetInstance()->ChangeScene("CLEAR");
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
		MyEngine::SceneManager::GetInstance()->ChangeScene("OVER");
	}
}
