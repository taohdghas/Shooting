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
	transform_.scale = { 2.0f, 2.0f, 2.0f };
}

//更新
void Boss1::Update() {
	//デスならスキップ
	if (is_dead_) {
		return;
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

	// 弾発射タイマー
	fire_timer_++;
	if (fire_timer_ >= kFireInterval) {
		FireDoubleHeightShot();
		fire_timer_ = 0;
	}
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
void Boss1::FireDoubleHeightShot()
{
	if (!player_) {
		return;
	}

	float yOffsets[] = { -0.5f, 0.8f };

	// 左・中央・右
	float angleOffsets[] = {
		-0.2f,
		 0.0f,
		 0.2f
	};

	const float bulletSpeed = 0.35f;

	Vector3 playerPos = player_->GetTranslate();

	for (float yOffset : yOffsets)
	{
		for (float angle : angleOffsets)
		{
			Vector3 spawnPos{
				transform_.translate.x,
				transform_.translate.y + yOffset,
				transform_.translate.z
			};

			// XZ平面のみで方向を作る
			Vector3 dir{
				playerPos.x - spawnPos.x,
				0.0f,
				playerPos.z - spawnPos.z
			};

			dir = Math::Normalize(dir);

			// スプレッド
			dir = Math::RotateY(dir, angle);

			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(object3d_base_);
			bullet->SetTranslate(spawnPos);
			bullet->SetVelocity(dir * bulletSpeed);
			bullet->Update();

			bullets_.push_back(std::move(bullet));
		}
	}
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