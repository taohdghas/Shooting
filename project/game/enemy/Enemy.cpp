#include "Enemy.h"
#include "Player.h"

//初期化
void Enemy::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemy.obj");
	object_->SetScale({ 0.5f,0.5f,0.5f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void Enemy::Update() {

	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}

	//攻撃(レーザー)
	Laser();

	object_->SetTranslate(transform_.translate);
	object_->Update();
}

//描画
void Enemy::Draw() {
	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}

	object_->Draw();
}

//攻撃(レーザー)
void Enemy::Laser() {
	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}

	//発射タイマー
	static int fireTimer = 0;
	fireTimer++;

	//インターバルよりも小さい場合スルー
	if (fireTimer < kFireInterval) {
		return;
	}



}

//衝突時コールバック
void Enemy::onCollision() {
	isDead_ = true;
}