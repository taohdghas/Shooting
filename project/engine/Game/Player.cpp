#include "Player.h"

//初期化
void Player::Initialize() {
	//モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(directxBase_);
	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
}
//更新
void Player::Update(Input* input) {
	object3d_->Update();
	//プレイヤーの移動
	if (input->PushKey(DIK_W)) {
		transform_.translate.y += speed;
	}
	if (input->PushKey(DIK_S)) {
		transform_.translate.y -= speed;
	}
	if (input->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}
	if (input->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}
}
//描画
void Player::Draw() {

}