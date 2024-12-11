#include "Player.h"

//初期化
void Player::Initialize() {
	//モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(directxBase_);
	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	//オブジェクト初期化
	object3d_->Initialize(object3dBase_);
	object3d_->SetModel("plane.obj");
	object3d_->SetTranslate({ 0.0f,0.0f,50.0f });
}
//更新
void Player::Update() {
	//デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});

	object3d_->Update();
	//プレイヤーの移動
	if (input_->PushKey(DIK_W)) {
		transform_.translate.y += speed;
	}
	if (input_->PushKey(DIK_S)) {
		transform_.translate.y -= speed;
	}
	if (input_->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}
	if (input_->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}
	//攻撃
	Attack();
	//弾の更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}
//描画
void Player::Draw() {
	//プレイヤーの描画
	object3d_->Draw();
	//model_->Draw();
	//プレイヤー弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();
	}
	//レティクル描画

}
//攻撃
void Player::Attack() {
	if(input_->TriggerKey(DIK_SPACE)){
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
	
	}
}