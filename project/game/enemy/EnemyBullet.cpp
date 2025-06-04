#include "EnemyBullet.h"

//初期化
void EnemyBullet::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemybullet.obj");
	object_->SetScale({ 1.0f,1.0f,1.0f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void EnemyBullet::Update() {

	transform_.translate = Math::Add(transform_.translate, velocity_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	object_->SetTranslate(transform_.translate);
	object_->Update();
}

//描画
void EnemyBullet::Draw() {
	
	object_->Draw();
}

//衝突時コールバック
void EnemyBullet::OnCollision() {
	isDead_ = true;
}