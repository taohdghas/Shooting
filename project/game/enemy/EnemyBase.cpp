#include "EnemyBase.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化処理
void EnemyBase::Initialize(MyEngine::Object3dBase* object3d_base) {
	object3d_base_ = object3d_base;

	object_ = std::make_unique<MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetLight(false);

	// 派生クラスで設定
	transform_.scale = default_scale_;
}

// 毎フレーム更新
void EnemyBase::Update() {
	if (is_dead_) {
		return;
	}

	// ダメージスケール演出
	if (damage_scale_timer_ > 0.0f) {
		damage_scale_timer_ -= kDeltaTime;
		float t = damage_scale_timer_ / kDamageScaleDuration;
		float scaleRate = 1.0f + 0.5f * t;
		transform_.scale = default_scale_ * scaleRate;
	} else {
		transform_.scale = default_scale_;
	}

	// ダメージカラー演出
	if (damage_color_timer_ > 0.0f) {
		damage_color_timer_ -= kDeltaTime;
		object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f });
	} else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);
	object_->Update();
}

// 描画処理
void EnemyBase::Draw() {
	if (is_dead_) {
		return;
	}
	object_->Draw();
}

// 衝突処理
void EnemyBase::OnCollision() {
	is_dead_ = true;
	is_death_particle_ = true;
}

// ダメージ処理
void EnemyBase::TakeDamage(int damage) {
	hp_ -= damage;

	damage_scale_timer_ = kDamageScaleDuration;
	damage_color_timer_ = kDamageColorDuration;

	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}

// デバッグ表示
void EnemyBase::Debug(int id) {
#ifdef USE_IMGUI
	std::string label = "Enemy##" + std::to_string(id);
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragInt("HP", &hp_, 1);
		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("Translate", &transform_.translate.x, 0.1f);
		ImGui::TreePop();
	}
#endif
}

// OBB取得
OBB EnemyBase::GetOBB() const {
	OBB obb;
	obb.center = transform_.translate;

	Matrix4x4 rot_mat = Math::MakeRotateMatrix(transform_.rotate);
	obb.orientations[0] = Math::Normalize({ rot_mat.m[0][0], rot_mat.m[1][0], rot_mat.m[2][0] });
	obb.orientations[1] = Math::Normalize({ rot_mat.m[0][1], rot_mat.m[1][1], rot_mat.m[2][1] });
	obb.orientations[2] = Math::Normalize({ rot_mat.m[0][2], rot_mat.m[1][2], rot_mat.m[2][2] });

	obb.size = (transform_.scale * dimensions_) * 0.5f;
	return obb;
}
