#include "playerBullet.h"
#include "Math.h"

//初期化
void playerBullet::Initialize(Object3d*object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
	object_->SetScale(Vector3{ 0.1f,0.1f,0.1f });
}

//更新
void playerBullet::Update() {

	if (isDead_) {
		return;
	}

	transform_.translate = Math::Add(transform_.translate, velocity_);

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