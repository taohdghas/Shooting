#include "Player.h"
#include "Input.h"

//初期化
void Player::Initialize(Object3d* object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
	//object_->SetScale(Vector3{0.2f,0.2f,0.2f});
}

//更新
void Player::Update() {


	//デスフラグが立った弾を削除
	bullets_.remove_if([](playerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	//移動
	Move();

	//弾の更新
	for (playerBullet* bullet : bullets_) {
		bullet->Update();
	}

	object_->SetTranslate(transform_.translate);

	object_->Update();
}

//描画
void Player::Draw() {

	//プレイヤー弾の描画
	for(playerBullet*bullet:bullets_){
		bullet->Draw();
	}

	object_->Draw();
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

	//弾の生成と初期化
	playerBullet* newBullet = new playerBullet();

}