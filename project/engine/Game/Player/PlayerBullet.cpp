#include "PlayerBullet.h"

//初期化
void PlayerBullet::Initialize() {
	//デスタイマー
	int32_t deathTimer = kLifeTime;
}
//更新
void PlayerBullet::Update() {
	//移動
	transform_.translate = Math::Add(transform_.translate, velocity_);
	//時間経過で消える
	if (--deathTimer < 0) {
		isDead_ = true;
	}
}
//描画
void PlayerBullet::Draw() {
	
}
