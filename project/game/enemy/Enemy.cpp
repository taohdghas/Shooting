#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "MyMath.h"

// 初期化処理
void Enemy::Initialize(MyEngine::Object3dBase* object3d_base) {
	// モデルの読み込み
	MyEngine::ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	// Object3dBaseの保存
	object3d_base_ = object3d_base;
	// 3Dオブジェクトの生成・初期化
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
	if (!rail_points_.empty())
	{
		// レール進行度を更新
		rail_progress_ += (rail_speed_ / rail_total_length_) * kDeltaTime;

		// 周回判定
		if (rail_progress_ >= 1.0f)
		{
			rail_progress_ -= 1.0f;
			rail_accumulated_ += rail_lap_offset_;
		}


		Vector3 rail_pos_ = EvaluateRailPosition(rail_progress_);
		Vector3 offset_ = rail_pos_ - rail_start_point_;
		// 最終的な座標を設定
		transform_.translate =
			rail_base_position_ + rail_accumulated_ + offset_;
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
}

// 描画処理
void Enemy::Draw() {
	if (is_dead_) {
		return;
	}

	// 敵本体の描画
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

	// 発射タイマー更新
	fire_timer_++;
	if (fire_timer_ < kFireInterval) {
		return;
	}

	// 敵からプレイヤーのベクトル
	Vector3 to_player = Math::Subtract(player_->GetTranslate(), transform_.translate);
	float distance = Math::Length(to_player);

	// 一定距離以上なら発射しない
	if (distance > kFireDistance) {
		return;
	}

	// プレイヤーより前に出すぎたら発射しない
	if (transform_.translate.z < player_->GetTranslate().z + kAttackStopDistanceZ) {
		return;
	}

	// 発射タイマーリセット
	fire_timer_ = 0;

	//距離依存でZ座標を先読み

	const float bulletSpeed = 0.5f;

	// 弾が到達するまでの時間（概算）
	float timeToHit = distance / bulletSpeed;

	// Z方向の先読み量（係数で調整）
	const float zLeadFactor = 0.08f; 
	float zLead = timeToHit * zLeadFactor;

	// 狙う位置
	Vector3 target = player_->GetTranslate();
	target.z += zLead;

	// 敵から狙い位置の方向
	Vector3 to_target = Math::Subtract(target, transform_.translate);
	Vector3 direction = Math::Normalize(to_target);

	// 弾生成
	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3d_base_);
	bullet->SetTranslate(transform_.translate);
	bullet->SetVelocity(Math::MultiplyScalar(direction, bulletSpeed));

	bullet->Update();
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
	size_t pointCount = rail_points_.size();

	float scaled = progress * (pointCount - 1);
	size_t index = (size_t)scaled;
	float t = scaled - index;

	// 範囲安全化
	size_t i0 = (index == 0) ? index : index - 1;
	size_t i1 = index;
	size_t i2 = std::min(index + 1, pointCount - 1);
	size_t i3 = std::min(index + 2, pointCount - 1);

	return Math::CatmullRom(
		rail_points_[i0],
		rail_points_[i1],
		rail_points_[i2],
		rail_points_[i3],
		t
	);
}

// OBB取得
OBB Enemy::GetOBB() const {
	OBB obb;
	obb.center = transform_.translate;

	Matrix4x4 rot_mat = Math::MakeRotateMatrix(transform_.rotate);
	obb.orientations[0] = Math::Normalize({ rot_mat.m[0][0], rot_mat.m[1][0], rot_mat.m[2][0] });
	obb.orientations[1] = Math::Normalize({ rot_mat.m[0][1], rot_mat.m[1][1], rot_mat.m[2][1] });
	obb.orientations[2] = Math::Normalize({ rot_mat.m[0][2], rot_mat.m[1][2], rot_mat.m[2][2] });

	obb.size.x = transform_.scale.x * dimensions_.x * 0.5f;
	obb.size.y = transform_.scale.y * dimensions_.y * 0.5f;
	obb.size.z = transform_.scale.z * dimensions_.z * 0.5f;

	return obb;
}
//敵の移動用レールを設定する
void Enemy::SetRail(const std::vector<Vector3>& controlPoints, bool closed)
{
	rail_points_ = controlPoints;
	rail_closed_ = closed;

	// 進行度リセット
	rail_progress_ = 0.0f;

	// 現在位置をレールの基準にする
	rail_base_position_ = transform_.translate;

	// レールの開始点
	rail_start_point_ = controlPoints.front();

	// 周回時のオフセット
	rail_lap_offset_ = controlPoints.back() - controlPoints.front();
	rail_accumulated_ = { 0.0f, 0.0f, 0.0f };

	// レール全長の計算
	rail_total_length_ = 0.0f;

	for (size_t i = 1; i < controlPoints.size(); ++i)
	{
		Vector3 diff = controlPoints[i] - controlPoints[i - 1];
		rail_total_length_ += Math::Length(diff);
	}

	// 閉じたレールなら最後→最初も加算
	if (rail_closed_ && controlPoints.size() >= 2)
	{
		Vector3 diff = controlPoints.front() - controlPoints.back();
		rail_total_length_ += Math::Length(diff);
	}
}


