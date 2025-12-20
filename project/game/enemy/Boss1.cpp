#include "Boss1.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化
void Boss1::Initialize(MyEngine::Object3dBase* object3d_base) {
	EnemyBase::Initialize(object3d_base);

	//object_->SetModel("enemy/boss1.obj");

	// ===== ボス専用ステータス =====
	hp_ = 300;
	attack_ = 20;
	radius_ = 3.0f;
	dimensions_ = 6.0f;

	default_scale_ = { 1.5f, 1.5f, 1.5f };
	transform_.scale = default_scale_;
	transform_.translate = { 0.0f, 5.0f, 30.0f };
}

// 更新
void Boss1::Update() {
	if (is_dead_) {
		return;
	}

	//共通処理
	EnemyBase::Update();

	// 弾更新
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		} else {
			++it;
		}
	}
}

// 描画
void Boss1::Draw() {
	if (is_dead_) {
		return;
	}

	EnemyBase::Draw();

	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

// デバッグ
void Boss1::Debug(int id) {
#ifdef USE_IMGUI
	std::string label = "Boss1##" + std::to_string(id);
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::Text("HP: %d", hp_);
		ImGui::TreePop();
	}
#endif
}
