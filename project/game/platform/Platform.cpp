#include "Platform.h"
#include "ImGuiManager.h"

//初期化
void Platform::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("platform/platform.obj");
	object_->SetLight(false);
	transform_.scale = { 7.5f,2.0f,3.0f };
	transform_.translate = { 0.0f,-1.9f,0.0f };
}
//更新
void Platform::Update(bool isStartAnimation, bool isReturning) {
	//スタート演出中は動かない
	if (isStartAnimation || isReturning) {
		object_->SetScale(transform_.scale);
		object_->SetRotate(transform_.rotate);
		object_->SetTranslate(transform_.translate);
		object_->Update();
		return;
	}

	//奥方向移動
	transform_.translate.z += moveSpeedz_;

	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void Platform::Draw() {
	object_->Draw();
}
//デバック
void Platform::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNodeEx("Platform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("PlatformScale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("PlatformRotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("PlatformTranslate", &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}