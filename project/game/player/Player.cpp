#include "Player.h"
#include "Input.h"

Player::Player() {}

Player::~Player() {
	//弾の解放
	for (playerBullet* bullet : bullets_) {
		delete bullet;
	}
}

//初期化
void Player::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("player/player.obj");
	object_->SetScale({ 0.1f,0.1f,0.1f });
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
	bullets_.remove_if([](playerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	Move();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		Attack();
	}

	if (Input::GetInstance()->PushKey(DIK_R)) {
		ThreeAttack();
	}

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();

	//弾の更新
	for (playerBullet* bullet : bullets_) {
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
	for (playerBullet* bullet : bullets_) {
		bullet->Draw();
	}
}

//移動
void Player::Move() {
	if (Input::GetInstance()->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		transform_.translate.y -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_W)) {
		transform_.translate.y += speed;
	}
}

//攻撃
void Player::Attack() {

	if (attackCooldown_ > 0) {
		return;
	}
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//弾を生成
	playerBullet* newBullet = new playerBullet();
	newBullet->Initialize(object3dBase_);
	newBullet->SetVelocity(velocity);
	newBullet->SetPosition(transform_.translate); 

	bullets_.push_back(newBullet);
	attackCooldown_ = attackInterval_;
}

//三方向攻撃
void Player::ThreeAttack() {
	if (attackCooldown_ > 0) {
		return;
	}

	const float kBulletSpeed = 1.0f;

	//発射方向
	std::vector<Vector3> directions = {
		Vector3(-1, 0, 1),  
		Vector3(0, 0, 1),   
		Vector3(1, 0, 1)    
	};

	for (const auto& dir : directions) {
		Vector3 velocity = Math::Normalize(dir); 
		velocity = Math::Multiply(velocity ,kBulletSpeed);     

		playerBullet* bullet = new playerBullet();
		bullet->Initialize(object3dBase_);
		bullet->SetVelocity(velocity);
		bullet->SetPosition(transform_.translate);
		bullets_.push_back(bullet);
	}

	attackCooldown_ = attackInterval_;
}

//衝突時コールバック
void Player::OnCollision() {
	isDead_ = true;
}