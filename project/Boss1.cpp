#include "Boss1.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化処理
void Boss1::Initialize(MyEngine::Object3dBase* object3d_base) {
	object3d_base_ = object3d_base;
	object_ = std::make_unique<MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("boss/boss.obj");
	object_->SetLight(false);

	// 乱数エンジン初期化
	std::random_device rd;
	random_engine_ = std::mt19937(rd());
	// 初期変換情報設定
	target_position_ = transform_.translate;
	// 最初の移動目標決定
	fire_interval_current_ = DecideFireInterval();
}

//更新
void Boss1::Update() {
	//デスならスキップ
	if (is_dead_) {
		return;
	}

	// 弾の更新とデスフラグ判定
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		} else {
			++it;
		}
	}

	Vector3 toTarget{
	target_position_.x - transform_.translate.x,
	target_position_.y - transform_.translate.y,
	0.0f
	};

	float distance = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

	if (distance < arrive_threshold_) {
		DecideNextTarget();
	} else {
		// 正規化
		toTarget.x /= distance;
		toTarget.y /= distance;

		// 目標速度
		Vector3 desiredVelocity{
			toTarget.x * max_speed_,
			toTarget.y * max_speed_,
			0.0f
		};

		// 現在速度 → 目標速度へ補間
		velocity_.x += (desiredVelocity.x - velocity_.x) * acceleration_ * kDeltaTime;
		velocity_.y += (desiredVelocity.y - velocity_.y) * acceleration_ * kDeltaTime;

		//速度減衰
		velocity_.x *= velocity_damping_;
		velocity_.y *= velocity_damping_;


		// 移動
		transform_.translate.x += velocity_.x * kDeltaTime;
		transform_.translate.y += velocity_.y * kDeltaTime;

		// 浮遊の揺らぎ
		float_time_ += kDeltaTime;

		// 揺らぎ量
		float swayX = std::sin(float_time_ * 1.3f) * 0.05f;
		float swayY = std::sin(float_time_ * 0.9f) * 0.08f;

		// 位置に加算
		transform_.translate.x += swayX;
		transform_.translate.y += swayY;

	}

	// 扇状拡散の遅延処理
	if (is_fan_shot_pending_) {
		fan_shot_delay_timer_++;

		if (fan_shot_delay_timer_ >= kFanShotDelay) {
			FireFanShot();
			is_fan_shot_pending_ = false;
			fan_shot_delay_timer_ = 0;
		}
	}
	fire_timer_++;

	if (fire_timer_ >= fire_interval_current_) {

		// 通常攻撃
		FireDoubleHeightShot();

		//体力が特定数値以下で攻撃追加
		if (hp_ <= 12 && !is_fan_shot_pending_) {
			is_fan_shot_pending_ = true;
		}

		// 次の発射間隔を再決定
		fire_interval_current_ = DecideFireInterval();

		fire_timer_ = 0;
	}

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

	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void Boss1::Draw() {
	if (is_dead_) {
		return;
	}
	object_->Draw();

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}
///二段高さショット発射
void Boss1::FireDoubleHeightShot() {
	if (!player_) {
		return;
	}

	//高さ
	float yOffsets[] = { -0.5f, 0.8f };
	//横方向
	float xOffsets[] = { -1.2f, 0.0f, 1.2f };

	const float bulletSpeed = 0.35f;

	//ボス中心 →プレイヤー
	Vector3 bossPos = transform_.translate;
	Vector3 playerPos = player_->GetTranslate();

	Vector3 baseDir{
		playerPos.x - bossPos.x,
		playerPos.y - bossPos.y,
		playerPos.z - bossPos.z
	};

	float length = std::sqrt(
		baseDir.x * baseDir.x +
		baseDir.y * baseDir.y +
		baseDir.z * baseDir.z
	);

	if (length != 0.0f) {
		baseDir.x /= length;
		baseDir.y /= length;
		baseDir.z /= length;
	}

	//全弾で同じ方向を使う
	Vector3 velocity{
		baseDir.x * bulletSpeed,
		baseDir.y * bulletSpeed,
		baseDir.z * bulletSpeed
	};

	for (float yOffset : yOffsets) {
		for (float xOffset : xOffsets) {

			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(object3d_base_);

			// 発射位置
			bullet->SetTranslate({
				bossPos.x + xOffset,
				bossPos.y + yOffset,
				bossPos.z
				});

			// 同一方向・直進
			bullet->SetVelocity(velocity);

			bullet->Update();
			bullets_.push_back(std::move(bullet));
		}
	}
}

/// 次の移動目標を決定
void Boss1::DecideNextTarget() {
	Vector3 newTarget;
	const int maxTry = 10;

	for (int i = 0; i < maxTry; ++i) {
		newTarget.x = RandomFloat(-7.0f, 7.0f);
		newTarget.y = RandomFloat(0.3f, 3.0f);
		newTarget.z = transform_.translate.z;

		Vector3 diff{
			newTarget.x - transform_.translate.x,
			newTarget.y - transform_.translate.y,
			0.0f
		};

		float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

		if (dist >= min_target_distance_) {
			target_position_ = newTarget;
			return;
		}
	}
	target_position_ = newTarget;
}
/// 扇状拡散ショット発射
void Boss1::FireFanShot()
{
	if (!player_) {
		return;
	}

	// 弾数と拡散角度
	const int kBulletCount = 7;
	const float kSpreadAngle = 20.0f; 
	const float bulletSpeed = 0.35f;

	Vector3 bossPos = transform_.translate;
	Vector3 playerPos = player_->GetTranslate();

	// 中央方向
	Vector3 baseDir{
		playerPos.x - bossPos.x,
		playerPos.y - bossPos.y,
		playerPos.z - bossPos.z
	};

	float length = std::sqrt(
		baseDir.x * baseDir.x +
		baseDir.y * baseDir.y +
		baseDir.z * baseDir.z
	);

	if (length == 0.0f) {
		return;
	}

	baseDir.x /= length;
	baseDir.y /= length;
	baseDir.z /= length;

	// 角度刻み
	float angleStep = (kSpreadAngle * 2.0f) / (kBulletCount - 1);

	for (int i = 0; i < kBulletCount; ++i) {
		float angle = -kSpreadAngle + angleStep * i;

		// ラジアン変換
		float rad = angle * (3.14159265f / 180.0f);

		// Y軸回転
		Vector3 dir{
			baseDir.x * std::cos(rad) - baseDir.z * std::sin(rad),
			baseDir.y,
			baseDir.x * std::sin(rad) + baseDir.z * std::cos(rad)
		};

		auto bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize(object3d_base_);
		bullet->SetTranslate(bossPos);
		bullet->SetVelocity({
			dir.x * bulletSpeed,
			dir.y * bulletSpeed,
			dir.z * bulletSpeed
			});

		bullet->Update();
		bullets_.push_back(std::move(bullet));
	}
}
///弾発射間隔決定
int Boss1::DecideFireInterval()
{
	// HPが多い間は固定
	if (hp_ > 12) {
		return kFireInterval;
	}

	// HP12以下：必ず早くなる範囲でランダム
	const int minInterval = 80;
	const int maxInterval = 120;

	std::uniform_int_distribution<int> dist(minInterval, maxInterval);
	return dist(random_engine_);
}

//衝突時コールバック
void Boss1::OnCollision() {
	is_dead_ = true;
	is_death_particle_ = true;
}
//ダメージ処理
void Boss1::TakeDamage(uint32_t damage) {
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
//デバッグ用ImGui表示
void Boss1::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Boss1")) {
		ImGui::DragInt("Boss1Hp", &hp_, 1);
		ImGui::DragFloat3("Boss1Scale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("Boss1Rotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("Boss1Translate", &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}

// OBB取得
OBB Boss1::GetOBB() const {
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
// min から max の範囲で乱数の浮動小数点数を生成
float Boss1::RandomFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random_engine_);
}
