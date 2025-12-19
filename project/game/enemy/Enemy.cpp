#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化
void Enemy::Initialize(MyEngine::Object3dBase* object3d_base) {
	EnemyBase::Initialize(object3d_base);

	object_->SetModel("enemy/enemy.obj");

	hp_ = 10;
	attack_ = 5;
	radius_ = 1.0f;
	dimensions_ = 2.0f;

	default_scale_ = { 0.5f, 0.5f, 0.5f };
	transform_.scale = default_scale_;
	transform_.translate = { 0.0f, 3.0f, 20.0f };
}

// 更新
void Enemy::Update() {
	//デスしたらスキップ
	if (is_dead_) {
		return;
	}

	//レール移動
	if (!rail_points_.empty()) {
		rail_progress_ += (rail_speed_ / rail_total_length_) * kDeltaTime;
		// 閉じたレールの場合ループ
		if (rail_progress_ >= 1.0f) {
			rail_progress_ -= 1.0f;
			rail_accumulated_ += rail_lap_offset_;
		}

		Vector3 rail_pos = EvaluateRailPosition(rail_progress_);
		Vector3 offset = rail_pos - rail_start_point_;
		transform_.translate =
			rail_base_position_ + rail_accumulated_ + offset;
	}

	//弾更新
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		} else {
			++it;
		}
	}

	//攻撃
	Laser();

	//共通更新
	EnemyBase::Update();

	//弾の再更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}
}

// 描画
void Enemy::Draw() {
	if (is_dead_) {
		return;
	}
	//共通描画
	EnemyBase::Draw();

	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

// レーザー攻撃
void Enemy::Laser() {
	if (!player_) {
		return;
	}

	fire_timer_++;
	if (fire_timer_ < kFireInterval) {
		return;
	}
	// プレイヤーまでのベクトルと距離を取得
	Vector3 to_player =
		Math::Subtract(player_->GetTranslate(), transform_.translate);
	float distance = Math::Length(to_player);
	// 一定距離以上離れている場合は攻撃しない
	if (distance > kFireDistance) {
		return;
	}
	// Z軸が近すぎる場合は攻撃しない
	if (transform_.translate.z <
		player_->GetTranslate().z + kAttackStopDistanceZ) {
		return;
	}

	fire_timer_ = 0;

	Vector3 direction = Math::Normalize(to_player);

	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3d_base_);
	bullet->SetTranslate(transform_.translate);
	bullet->SetVelocity(Math::MultiplyScalar(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}

// レール評価
Vector3 Enemy::EvaluateRailPosition(float progress) {
	size_t count = rail_points_.size();

	float scaled = progress * (count - 1);
	size_t index = (size_t)scaled;
	float t = scaled - index;

	size_t i0 = (index == 0) ? index : index - 1;
	size_t i1 = index;
	size_t i2 = min(index + 1, count - 1);
	size_t i3 = min(index + 2, count - 1);

	return Math::CatmullRom(
		rail_points_[i0],
		rail_points_[i1],
		rail_points_[i2],
		rail_points_[i3],
		t
	);
}

// レール設定
void Enemy::SetRail(const std::vector<Vector3>& controlPoints, bool closed) {
	rail_points_ = controlPoints;
	rail_closed_ = closed;

	rail_progress_ = 0.0f;
	rail_base_position_ = transform_.translate;
	rail_start_point_ = controlPoints.front();
	rail_lap_offset_ = controlPoints.back() - controlPoints.front();
	rail_accumulated_ = { 0,0,0 };

	rail_total_length_ = 0.0f;
	for (size_t i = 1; i < controlPoints.size(); ++i) {
		rail_total_length_ +=
			Math::Length(controlPoints[i] - controlPoints[i - 1]);
	}

	if (rail_closed_ && controlPoints.size() >= 2) {
		rail_total_length_ +=
			Math::Length(controlPoints.front() - controlPoints.back());
	}
}

// デバッグ
void Enemy::Debug(int id) {
	EnemyBase::Debug(id);
}
