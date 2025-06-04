#include "playerBullet.h"
#include "Math.h"

//初期化
void playerBullet::Initialize(Object3dBase* object3dbase) {
	object3dbase_ = object3dbase;

	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dbase_);
	object_->SetModel("playerbullet.obj");
	object_->SetScale({ 1.0f,1.0f,1.0f });

	//デスタイマー
	deathTimer = kLifeTime;
}
//更新
void playerBullet::Update() {
	if (isDead_) {
		return;
	}
	//移動
	transform_.translate = Math::Add(transform_.translate, velocity_);
	//時間経過で消える
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void playerBullet::Draw() {
	if (isDead_) {
		return;
	}
	object_->Draw();
}
//衝突時コールバック関数
void playerBullet::OnCollision() {
	isDead_ = true;
}