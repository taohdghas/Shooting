#include "Ui.h"
#include "SpriteBase.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <algorithm>

//初期化
void Ui::Initialize()
{
	//HPバーの初期化
    hp_bar_ = std::make_unique<MyEngine::Sprite>();
    hp_bar_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/hpbar.png");
    hp_bar_->SetSize({ 200, 20 });
    hp_bar_->SetPosition({ 50, 50 });

	//ポーズの初期化
	pause_ = std::make_unique<MyEngine::Sprite>();
	pause_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/pause.png");
    pause_->SetSize({ 300,60 });
	pause_->SetPosition({ 1000, 20 });

	//操作説明画面の初期化
    operation_guide_ = std::make_unique<MyEngine::Sprite>();
    operation_guide_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/operation.png");
    operation_guide_->SetPosition({ 640, 360 }); 
    operation_guide_->SetSize({ 600, 600 });
	operation_guide_->SetAnchorPoint({ 0.5f, 0.5f });
}
//更新
void Ui::Update()
{
	//HPバー処理
    UpdateHPBar();
    //操作説明画面処理
	UpdateOperationGuide();
    //ポーズ更新
	pause_->Update();
    //HPバー更新
    hp_bar_->Update();
}
//描画
void Ui::Draw()
{
	//HPバー描画
    hp_bar_->Draw();
	//ポーズ描画
	pause_->Draw();

	//操作説明画面描画
    if (operation_scale_ > 0.01f) {
        operation_guide_->Draw();
    }
}
//デバック
void Ui::Debug()
{
#ifdef USE_IMGUI
    Vector2 position = hp_bar_->GetPosition();
    Vector2 size = hp_bar_->GetSize();

    if (ImGui::Begin("HPBar Debug")) {
        if (ImGui::TreeNode("HPBar")) {
            ImGui::DragFloat2("Position", &position.x, 1.0f);
            ImGui::DragFloat2("Size", &size.x, 1.0f);

            hp_bar_->SetPosition(position);
            hp_bar_->SetSize(size);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Pause")) {
            Vector2 pause_pos = pause_->GetPosition();
            Vector2 pause_size = pause_->GetSize();

            ImGui::DragFloat2("Position", &pause_pos.x, 1.0f);
            ImGui::DragFloat2("Size", &pause_size.x, 1.0f);

            pause_->SetPosition(pause_pos);
            pause_->SetSize(pause_size);

            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Operation Guide")) {
            // SRT取得
            Vector2 op_pos = operation_guide_->GetPosition();
            Vector2 op_size = operation_guide_->GetSize();
            float op_rot = operation_guide_->GetRotation();

            // IMGUIで編集
            ImGui::DragFloat2("Position", &op_pos.x, 1.0f);
            ImGui::DragFloat2("Size", &op_size.x, 1.0f);
            ImGui::DragFloat("Rotation", &op_rot, 0.01f);

            // 反映
            operation_guide_->SetPosition(op_pos);
            operation_guide_->SetSize(op_size);
            operation_guide_->SetRotation(op_rot);

            ImGui::TreePop();
        }
    }
    ImGui::End();
#endif
}
//HPバー更新
void Ui::UpdateHPBar() {

    //現在のHPを取得
    float currentHP = static_cast<float>(player_->GetHP());

    //HPの割合
    constexpr float maxHPValue = 100.0f;
    float hpRatio = std::clamp(currentHP / maxHPValue, 0.0f, 1.0f);

    //最大サイズ
    Vector2 maxSize = { 200.0f, 20.0f };

    //HP割合に応じて横幅を調整
    hp_bar_->SetSize({ maxSize.x * hpRatio, maxSize.y });
}
//操作説明画面処理
void Ui::UpdateOperationGuide()
{
    const float speed = 0.07f;
    
	//スケール更新
    if (is_show_operation_) {
        operation_scale_ += speed;
    } else {
        operation_scale_ -= speed;
    }

	//範囲制限
    operation_scale_ = std::clamp(operation_scale_, 0.0f, 1.0f);
	//イージング適用
    float eased = static_cast<float>(easeOutQuad(operation_scale_));

    Vector2 baseSize = { 600.0f, 600.0f };
    operation_guide_->SetSize(Math::MultiplyScalar(baseSize,eased));

    operation_guide_->Update();
}
