#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化処理
void Enemy::Initialize(MyEngine::Object3dBase* object3d_base) {
	object3d_base_ = object3d_base;
	object_ = std::make_unique<MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("enemy/enemy.obj");
	object_->SetLight(false);
	transform_.scale = { 0.5f, 0.5f, 0.5f };
	transform_.translate = { 0.0f, 3.0f, 20.0f };
}

// 毎フレームの更新処理
void Enemy::Update() {
	//デスならスキップ
	if (is_dead_) {
		return;
	}
	
	// スプライン移動がある場合のみ処理
	if (!railPoints_.empty())
	{
		rail_progress_ += rail_speed_ * kDeltaTime;

		// 周回判定
		if (rail_progress_ >= 1.0f)
		{
			rail_progress_ -= 1.0f;

			// 累積オフセットに1周分を加算
			rail_accumulated_ += rail_lap_offset_;
		}

		Vector3 railPos = EvaluateRailPosition(rail_progress_);
		Vector3 offset = railPos - rail_start_point_;
		// 最終的な座標を設定
		transform_.translate =
			rail_base_position_ + rail_accumulated_ + offset;
	}

	
	// 弾の更新とデスフラグ判定（デッドならリストから削除）
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		} else {
			++it;
		}
	}

	// 攻撃処理（レーザー発射判定）
	Laser();

	// ダメージスケール処理
	if (damage_scale_timer_ > 0.0f) {
		damage_scale_timer_ -= kDeltaTime;

		// 拡大してから戻る
		float t = (damage_scale_timer_ / damage_scale_duration_);
		float scaleRate = 1.0f + 0.5f * t;

		transform_.scale = default_scale_ * scaleRate;
	} else {
		transform_.scale = default_scale_;
	}

	// ダメージ時の色変更タイマー処理
	if (damage_color_timer_ > 0.0f) {
		damage_color_timer_ -= kDeltaTime;
		object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f });
	} else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	// Transform情報をObject3dへ反映
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

	object_->Update();

	// 弾の再更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}
}

// 描画処理
void Enemy::Draw() {
	if (is_dead_) {
		return;
	}

	object_->Draw();

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

// レーザー攻撃判定・発射処理
void Enemy::Laser() {
	if (is_dead_ || !player_) {
		return;
	}

	static int fire_timer = 0;
	fire_timer++;

	// 発射間隔未満なら発射しない
	if (fire_timer < kFireInterval) {
		return;
	}

	// プレイヤーとの距離計算
	Vector3 to_player = Math::Subtract(player_->GetTranslate(), transform_.translate);
	float distance = Math::Length(to_player);

	// 一定距離以上なら発射しない
	if (distance > kFireDistance) {
		return;
	}

	// プレイヤーのZ座標＋定数より手前なら発射しない
	if (transform_.translate.z < player_->GetTranslate().z + kAttackStopDistanceZ) {
		return;
	}

	// 発射タイマーリセット
	fire_timer = 0;

	// プレイヤー方向に正規化ベクトルを算出
	Vector3 direction = Math::Normalize(to_player);

	// 弾生成・初期化・速度設定
	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3d_base_);
	bullet->SetTranslate(transform_.translate);
	bullet->SetVelocity(Math::MultiplyScalar(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}

// 衝突時の処理（死亡フラグ・パーティクル発生）
void Enemy::OnCollision() {
	is_dead_ = true;
	is_death_particle_ = true;
}

// ダメージ処理
void Enemy::TakeDamage(int damage) {
	hp_ -= damage;
	// スケール演出タイマーセット
	damage_scale_timer_ = kDamageScaleDuration;
	// ダメージ色タイマーセット
	damage_color_timer_ = kDamageColorDuration;
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision(); // HP0で死亡処理
	}
}

// デバッグ用ImGui表示
void Enemy::Debug(int id) {
#ifdef USE_IMGUI
	std::string tree_label = "Enemy##" + std::to_string(id);
	if (ImGui::TreeNode(tree_label.c_str())) {
		std::string hp_label = "EnemyHp##" + std::to_string(id);
		std::string scale_label = "EnemyScale##" + std::to_string(id);
		std::string rotate_label = "EnemyRotate##" + std::to_string(id);
		std::string translate_label = "EnemyTranslate##" + std::to_string(id);

		ImGui::DragInt(hp_label.c_str(), &hp_, 1);
		ImGui::DragFloat3(scale_label.c_str(), &transform_.scale.x, 0.1f);
		ImGui::DragFloat3(rotate_label.c_str(), &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3(translate_label.c_str(), &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}

// レール上の位置を評価する
Vector3 Enemy::EvaluateRailPosition(float progress)
{
	size_t pointCount = railPoints_.size();

	float scaled = progress * (pointCount - 1);
	size_t index = (size_t)scaled;
	float t = scaled - index;

	// 範囲安全化
	size_t i0 = (index == 0) ? index : index - 1;
	size_t i1 = index;
	size_t i2 = min(index + 1, pointCount - 1);
	size_t i3 = min(index + 2, pointCount - 1);

	return Math::CatmullRom(
		railPoints_[i0],
		railPoints_[i1],
		railPoints_[i2],
		railPoints_[i3],
		t
	);
}

// OBB（当たり判定用の回転付きボックス）取得
OBB Enemy::GetOBB() const {
	OBB obb;
	obb.center = transform_.translate;

	// 回転行列から各軸ベクトルを算出・正規化
	Matrix4x4 rot_mat = Math::MakeRotateMatrix(transform_.rotate);
	obb.orientations[0] = Math::Normalize({ rot_mat.m[0][0], rot_mat.m[1][0], rot_mat.m[2][0] }); // X軸
	obb.orientations[1] = Math::Normalize({ rot_mat.m[0][1], rot_mat.m[1][1], rot_mat.m[2][1] }); // Y軸
	obb.orientations[2] = Math::Normalize({ rot_mat.m[0][2], rot_mat.m[1][2], rot_mat.m[2][2] }); // Z軸

	// スケールと寸法からサイズ算出
	obb.size = (transform_.scale * dimensions_) * 0.5f;

	return obb;
}
//敵の移動用レール（スプライン）の制御点と閉じているかどうかを設定する
void Enemy::SetRail(const std::vector<Vector3>& controlPoints, bool closed)
{
	railPoints_ = controlPoints;
	rail_closed_ = closed;

	rail_progress_ = 0.0f;
	
	rail_base_position_ = transform_.translate;
	rail_start_point_ = controlPoints.front();
	rail_lap_offset_ = controlPoints.back() - controlPoints.front();

	rail_accumulated_ = { 0, 0, 0 };
}

