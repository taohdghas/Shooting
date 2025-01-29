#include "skydome.h"

void skydome::Initialize(Object3dBase*object3dbase) {
	object3dBase_ = object3dbase;
	ModelManager::GetInstance()->Initialize(directxBase_);
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("skydome.obj");
	transform_.translate = { 150.0f,0.0f,0.0f };
	transform_.scale = { 6.0f,6.0f,6.0f };
}

void skydome::Update() {
	object_->SetTranslate(transform_.translate);
	object_->SetScale(transform_.scale);
	object_->Update();
}

void skydome::Draw() {
	object_->Draw();
}