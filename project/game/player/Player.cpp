#include "Player.h"
#include "Input.h"

//初期化
void Player::Initialize(Object3d* object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
	object_->SetScale(Vector3{0.2f,0.2f,0.2f});
}

//更新
void Player::Update() {

	//移動
	Move();

	object_->Update();
}

//描画
void Player::Draw() {
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

	object_->SetTranslate(transform_.translate);
}

//攻撃
void Player::Attack() {

}