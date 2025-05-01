#include "Player.h"

//初期化
void Player::Initialize(Object3d* object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
}

//更新
void Player::Update() {
	object_->Update();
}

//描画
void Player::Draw() {
	object_->Draw();
}

//移動
void Player::Move() {
	transform_.translate.x += speed;
	transform_.translate.y += speed;

	object_->SetTranslate(transform_.translate);
}