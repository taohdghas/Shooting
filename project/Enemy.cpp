#include "Enemy.h"
#include "Player.h"
#include "Math.h"

// 追尾をやめる距離
const float Enemy::kApproachDistance = 3.0f;

Enemy::Enemy() {}

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
	object_->SetModel("enemy.obj");
	object_->SetScale({ 0.5f,0.5f,0.5f });
	transform_.translate = { 0.0f,0.0f,30.0f };
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
	transform_.translate = Math::Add(transform_.translate, velocity_);

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();
	Laser();
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

	auto bullet = std::make_unique<enemybullet>();
	bullet->Initialize(object3dBase_);
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}
//追尾弾フェーズ
void Enemy::Homing() {
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