#include "Player.h"

Player::Player(){}

Player::~Player() {
	//弾の解放
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

//初期化
void Player::Initialize(Object3dBase* object3dbase, Camera* camera) {
	object3dBase_ = object3dbase;
	camera_ = camera;
	isDead_ = false;
	hp_ = 100;
	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("mm_frame.obj");
	object_->SetScale({ 0.6f,0.6f,0.6f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}
//更新
void Player::Update() {
	if (isDead_) {
		return;
	}
	if (attackCooldown_ > 0) {
		attackCooldown_--;
	}
	//デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	transform_.translate.z = camera_->GetTranslate().z + 10.0f;

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();

	//弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	
}
//描画
void Player::Draw() {
	if (isDead_) {
		return;
	}
	//プレイヤーの描画
	object_->Draw();
	
	//プレイヤー弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
}
//攻撃
void Player::Attack() {
	if (attackCooldown_ > 0) {
		return; 
	}
	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 弾を生成し初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(object3dBase_);
	newBullet->SetVelocity(velocity);
	newBullet->SetPosition(transform_.translate); // プレイヤーの位置に弾を設定

	// 弾を登録
	bullets_.push_back(newBullet);
	attackCooldown_ = attackInterval_;
}
//衝突時コールバック関数
void Player::OnCollision() {
	isDead_ = true;
}
//ダメージ
void Player::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}
//移動
void Player::MoveUp() {
	this->transform_.translate.y += this->speed;
}
void Player::MoveDown() {
	this->transform_.translate.y -= this->speed;
}
void Player::MoveLeft() {
	this->transform_.translate.x -= this->speed;
}
void Player::MoveRight() {
	this->transform_.translate.x += this->speed;
}