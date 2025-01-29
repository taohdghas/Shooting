#include "enemybullet.h"

//初期化
void enemybullet::Initialize(Object3dBase* object3dbase) {
	object3dbase_ = object3dbase;

	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dbase_);
	object_->SetModel("bullet.obj");
	object_->SetScale({ 0.3f,0.3f,0.3f });
	transform_.translate = { 0.0f, 0.0f, 0.0f };
	deathTimer = kLifeTime;
}
//更新
void enemybullet::Update() {

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
void enemybullet::Draw() {
	object_->Draw();
}
//衝突時コールバック関数
void enemybullet::OnCollision() {
	isDead_ = true;
}