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

	//操作説明画面の初期化
    operation_guide_ = std::make_unique<MyEngine::Sprite>();
    operation_guide_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/player.png");
    operation_guide_->SetPosition({ 640, 360 }); 
    operation_guide_->SetSize({ 600, 400 });
	operation_guide_->SetAnchorPoint({ 0.5f, 0.5f });
}
//更新
void Ui::Update()
{
	//HPバー処理
    UpdateHPBar();
    //操作説明画面処理
	UpdateOperationGuide();
    //HPバー更新
    hp_bar_->Update();
}
//描画
void Ui::Draw()
{
	//HPバー描画
    hp_bar_->Draw();

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

    Vector2 baseSize = { 600.0f, 400.0f };
    operation_guide_->SetSize(Math::MultiplyScalar(baseSize,eased));

    operation_guide_->Update();
}
