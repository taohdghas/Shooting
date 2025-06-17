#include "Skydome.h"

//初期化
void Skydome::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("skydome/skydome.obj");
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void Skydome::Update() {

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();
}

//描画
void Skydome::Draw() {
	object_->Draw();
}
