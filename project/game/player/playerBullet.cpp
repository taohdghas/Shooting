#include "playerBullet.h"
#include "Math.h"

//初期化
void playerBullet::Initialize(std::unique_ptr<Object3d>object) {
	object_ = std::move(object);
	object_->SetScale(Vector3{ 0.05f,0.05f,0.05f });
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