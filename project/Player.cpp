#include "Player.h"

Player::Player(){}

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Object3dBase*object3dbase) {
	object3dBase_ = object3dbase;

	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("mm_frame.obj");
	object_->SetScale({ 0.8f,0.8f,0.8f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

void Player::Update() {
	if (isDead_) {
		return;
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	//transform_.translate.z = camera_->GetTranslate().z + 10.0f;

	if (Input::GetInstance()->PushKey(DIK_W)) {
		transform_.translate.y += speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_S)) {
		transform_.translate.y -= speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Draw() {
	if (isDead_) {
		return;
	}
	//プレイヤーの描画
	object_->Draw();

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
}

void Player::Attack() {
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
}

void Player::OnCollision() {
	isDead_ = true;
}

void Player::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}