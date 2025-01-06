#include "Title.h"

//初期化
void Title::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	//モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(directxBase_);
	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("text.obj");
	transform_.translate = { -3.5f,2.0f,0.0f };
}
//更新
void Title::Update() {
	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void Title::Draw() {
	object_->Draw();
}
