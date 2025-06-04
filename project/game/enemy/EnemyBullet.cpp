#include "EnemyBullet.h"

//初期化
void EnemyBullet::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemybullet.obj");
	object_->SetScale({ 1.0f,1.0f,1.0f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void EnemyBullet::Update() {

	object_->SetTranslate(transform_.translate);
	object_->Update();
}

//描画
void EnemyBullet::Draw() {
	
	object_->Draw();
}