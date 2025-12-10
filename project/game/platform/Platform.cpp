#include "Platform.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// プラットフォームの初期化処理
void Platform::Initialize(MyEngine::Object3dBase* object3dbase) {
	object3d_base_ = object3dbase;
	object_ = std::make_unique< MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("platform/platform.obj");
	object_->SetLight(false);
	transform_.scale = { 7.5f,2.0f,3.0f };
	transform_.translate = { 0.0f,-1.9f,0.0f };

	//煙パーティクルエミッター初期化
	smoke_emitter_.Initialize("platformsmoke");
}

// 毎フレームの更新処理（Transform情報をObject3dへ反映）
void Platform::Update(bool is_start_animation_, bool is_returning_) {
	//スタート演出中は動かない
	if (is_start_animation_ || is_returning_) {
		object_->SetScale(transform_.scale);
		object_->SetRotate(transform_.rotate);
		object_->SetTranslate(transform_.translate);
		object_->Update();
		return;
	}
	
	//奥方向移動
	transform_.translate.z += move_speed_z_;

	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);
	object_->Update();
	
	//煙パーティクルエミッター処理
	Vector3 smoke_pos = transform_.translate + Vector3{ 0.0f,0.0f,-1.5f };
	smoke_emitter_.SetPosition(smoke_pos);
	smoke_emitter_.Update();
}

// プラットフォームの描画処理
void Platform::Draw() {
	object_->Draw();
}

// デバッグ用ImGui表示（Transformのパラメータ調整）
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