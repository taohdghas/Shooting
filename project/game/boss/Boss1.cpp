#include "Boss1.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "ModelManager.h"
#include "MyMath.h"
#include "BossStateNormal.h"

// 初期化処理
void Boss1::Initialize(MyEngine::Object3dBase* object3d_base) {
	// モデルの読み込み
	MyEngine::ModelManager::GetInstance()->LoadModel("boss/boss.obj");
	// Object3dBaseの保存
	object3d_base_ = object3d_base;
	// 3Dオブジェクトの生成・初期化
	object_ = std::make_unique<MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("boss/boss.obj");
	object_->SetLight(false);

	// 乱数エンジン初期化
	std::random_device rd;
	random_engine_ = std::mt19937(rd());
	// 初期変換情報設定
	target_position_ = transform_.translate;
	//状態を通常状態に設定
	ChangeState(std::make_unique<BossStateNormal>());
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
	//状態更新
	if (state_) {
		state_->Update(*this);
	}

	//ダメージスケール処理
	if (damage_scale_timer_ > 0.0f) {
		damage_scale_timer_ -= kDeltaTime;

		//拡大してから拡縮
		float t = (damage_scale_timer_ / damage_scale_duration_);
		float scaleRate = 1.0f + 0.5f * t;

		transform_.scale = default_scale_ * scaleRate;
	} else {
		transform_.scale = default_scale_;
	}

	//ダメージ時の色変更タイマー処理
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
	//本体の描画
	object_->Draw();

	//弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}
//移動
void Boss1::Move() {
	//目標へのベクトル
	Vector3 toTarget{
	target_position_.x - transform_.translate.x,
	target_position_.y - transform_.translate.y,
	0.0f
	};
	//目標までの距離
	float distance = Math::Length(toTarget);

	//目標到達判定
	if (distance < arrive_threshold_) {
		DecideNextTarget();
	} else {
		//正規化
		toTarget = Math::Normalize(toTarget);

		//目標速度
		Vector3 desiredVelocity{
			toTarget.x * max_speed_,
			toTarget.y * max_speed_,
			0.0f
		};

		// 現在速度から目標速度へ補間
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
}

///二段高さショット発射
void Boss1::FireDoubleHeightShot() {

	if (!player_) {
		return;
	}

	// 定数化したオフセットを使用
	constexpr size_t yCount = std::size(kDoubleShotYOffset);
	constexpr size_t xCount = std::size(kDoubleShotXOffset);

	Vector3 bossPos = transform_.translate;
	Vector3 playerPos = player_->GetTranslate();

	// 二段に分けて発射
	for (size_t yIdx = 0; yIdx < yCount; ++yIdx) {
		float yOffset = kDoubleShotYOffset[yIdx];
		for (size_t xIdx = 0; xIdx < xCount; ++xIdx) {
			float xOffset = kDoubleShotXOffset[xIdx];

			//発射位置にランダム性を追加
			Vector3 spawnPos{
				bossPos.x + xOffset + RandomFloat(-10.0f, 10.0f),
				bossPos.y + yOffset + RandomFloat(-4.0f, 4.0f),
				bossPos.z
			};

			// 弾ごとにプレイヤーを狙う
			Vector3 dir{
				playerPos.x - spawnPos.x,
				playerPos.y - spawnPos.y,
				playerPos.z - spawnPos.z
			};

			if (Math::Length(dir) == 0.0f) continue;

			dir = Math::Normalize(dir);

			auto bullet = std::make_unique<EnemyBullet>();
			bullet->Initialize(object3d_base_);
			bullet->SetTranslate(spawnPos);
			bullet->SetVelocity({
				dir.x * kBulletSpeed,
				dir.y * kBulletSpeed,
				dir.z * kBulletSpeed
				});
			bullet->Update();
			bullets_.push_back(std::move(bullet));
		}
	}
}

/// 次の移動目標を決定
void Boss1::DecideNextTarget() {
	Vector3 newTarget;
	const int maxTry = 10;

	// 十分離れた位置を探す
	for (int i = 0; i < maxTry; ++i) {
		newTarget.x = RandomFloat(-6.0f, 6.0f);
		newTarget.y = RandomFloat(0.0f, 0.1f);
		newTarget.z = transform_.translate.z;

		// 現在位置からの距離を計算
		Vector3 diff{
			newTarget.x - transform_.translate.x,
			newTarget.y - transform_.translate.y,
			0.0f
		};

		// 距離計算
		float dist = Math::Length(diff);

		// 十分離れていれば決定
		if (dist >= min_target_distance_) {
			target_position_ = newTarget;
			return;
		}
	}
	// 十分離れた位置が見つからなかった場合は最後に決定した位置を使用
	target_position_ = newTarget;
}
/// 扇状拡散ショット発射
void Boss1::FireFanShot()
{
	if (!player_) {
		return;
	}

	// 弾数
	const int kBulletCount = 7;
	// 20度ずつ左右に拡散
	const float kSpreadAngle = 20.0f; 
	// 弾速
	const float bulletSpeed = 0.25;

	Vector3 bossPos = transform_.translate;
	Vector3 playerPos = player_->GetTranslate();

	// 中央方向
	Vector3 baseDir{
		playerPos.x - bossPos.x,
		playerPos.y - bossPos.y,
		playerPos.z - bossPos.z
	};

	if (Math::Length(baseDir) == 0.0f) {
		return;
	}
	// 正規化
	baseDir = Math::Normalize(baseDir);

	// 角度刻み
	float angleStep = (kSpreadAngle * 2.0f) / (kBulletCount - 1);

	// 弾発射ループ
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
		bullet->SetColor({ 0.2f, 0.6f, 1.0f, 1.0f });
		bullet->Update();
		bullets_.push_back(std::move(bullet));
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
///状態変更
void Boss1::ChangeState(std::unique_ptr<BossState> newState) {
	if (state_) state_->Exit(*this);
	state_ = std::move(newState);
	if (state_) state_->Enter(*this);
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

///弾発射間隔決定
int Boss1::DecideFireInterval()
{
	// HPが多い間は固定
	if (hp_ > kEnragedHP) {
		return kFireInterval;
	}
	const int minInterval = 70;
	const int maxInterval = 100;

	std::uniform_int_distribution<int> dist(minInterval, maxInterval);
	return dist(random_engine_);
}

// min から max の範囲で乱数の浮動小数点数を生成
float Boss1::RandomFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(random_engine_);
}

//タイマーの共通関数
bool Boss1::UpdateDelayTimer(int& timer, int threshold) {
	timer++;
	if (timer >= threshold) {
		timer = 0;
		return true;
	}
	return false;
}