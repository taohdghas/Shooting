#include "Enemy.h"
#include "Player.h"
#include "Math.h"

// 追尾をやめる距離
const float Enemy::kApproachDistance = 3.0f;

constexpr float PI = 3.14159265358979323846f;

float ToRadians(float degrees) {
	return degrees * (PI / 180.0f);
}


Enemy::Enemy(){}

Enemy::~Enemy() {
	
}

//初期化
void Enemy::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	isDead_ = false;
	hp_ = 100;
	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("axis.obj");
	object_->SetScale({ 0.5f,0.5f,0.5f });
	transform_.translate = { 0.0f,0.0f,20.0f };
	initialPositionX_ = transform_.translate.x;
}
//更新
void Enemy::Update() {
	//フラグがたったらスキップ
	if (isDead_) {
		return;
	}
	for (auto it = bullets_.begin(); it != bullets_.end(); ) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}
	//移動
	 // 左右移動
	transform_.translate.x += moveDirection_ * moveSpeed_;

	// 移動範囲を超えたら方向を反転
	if (transform_.translate.x <= initialPositionX_ - moveRange_ ||
		transform_.translate.x >= initialPositionX_ + moveRange_) {
		moveDirection_ *= -1;
	}

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();

	fireTimer_++;
	if (currentPhase_ == Phase::Laser) {
		if (fireTimer_ >= kFireInterval) {
			fireTimer_ = 0;
			currentPhase_ = Phase::RotateShot;
		}
		Laser();
	}
	else if (currentPhase_ == Phase::RotateShot) {
		if (fireTimer_ >= kRotateFireInterval * 72) {  
			fireTimer_ = 0;
			rotateAngle_ = 0;
			currentPhase_ = Phase::Laser;
		}
		RotateShot();
	}
}
//描画
void Enemy::Draw() {
	//フラグがたったらスキップ
	if (isDead_) {
		return;
	}
	object_->Draw();
	//弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}
//レーザーフェーズ
void Enemy::Laser() {

	static int fireTimer = 0;
	fireTimer++;
	if (fireTimer < kFireInterval) {
		return;
	}
	fireTimer = 0;

	if (!player_) return;

	Vector3 direction = Math::Subtract(player_->GetPosition(), transform_.translate);
	direction = Math::Normalize(direction);

	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3dBase_);
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}
void Enemy::RotateShot() {

	if (fireTimer_ % kRotateFireInterval != 0) return;

	float radians = ToRadians(float(rotateAngle_));
	Vector3 direction = { cosf(radians), 0, sinf(radians) };
	direction = Math::Normalize(direction);

	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3dBase_);
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.3f));

	bullets_.emplace_back(std::move(bullet));

	rotateAngle_ += 15;  // 5°ずつ回転
}
//HP減少関数
void Enemy::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}
//衝突時コールバック関数
void Enemy::OnCollision() {
	isDead_ = true;
}