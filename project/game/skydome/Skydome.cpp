#include "Skydome.h"

//初期化
void Skydome::Initialize(Object3d* object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
}

//更新
void Skydome::Update() {
	object_->Update();
}

//描画
void Skydome::Draw() {
	object_->Draw();
}
