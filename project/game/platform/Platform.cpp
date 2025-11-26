#include "Platform.h"
#include "ParticleManager.h"
#include "MyMath.h"
#include "ImGuiManager.h"

// プラットフォームの初期化処理
void Platform::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("platform/platform.obj");
	object_->SetLight(false);
	transform_.scale = { 7.5f,2.0f,3.0f };
	transform_.translate = { 0.0f,-1.9f,0.0f };

	smokeEmitter_.Initialize("particle6");
}

// 毎フレームの更新処理
void Platform::Update() {
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);
	object_->Update();

	Vector3 smokePos = transform_.translate + Vector3(0.0f, 0.0f, -1.5f);
	smokeEmitter_.SetPosition(smokePos);
	smokeEmitter_.Update();
}

// プラットフォームの描画処理
void Platform::Draw() {
	object_->Draw();
}

// デバッグ用ImGui表示（Transformのパラメータ調整）
void Platform::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Platform")) {
		ImGui::DragFloat3("PlatformScale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("PlatformRotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("PlatformTranslate", &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}