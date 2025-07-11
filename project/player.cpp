#include "player.h"

player::player() {}

void player::Initialize(Object3dBase*object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);

}

void player::Update() {
	object_->SetTranslate(transform_.translate);
	object_->Update();
}

void player::Draw() {
	object_->Draw();
}