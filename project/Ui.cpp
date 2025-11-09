#include "Ui.h"
#include "SpriteBase.h"
#include "Input.h"
#include "ImGuiManager.h"
#include <algorithm>

//初期化
void Ui::Initialize()
{
	hPBar_ = std::make_unique<Sprite>();
	hPBar_->Initialize(SpriteBase::GetInstance(), "resources/ui/hpbar.png");
    hPBar_->SetSize({ 200, 20 });
    hPBar_->SetPosition({ 50, 50 });
}
//更新
void Ui::Update()
{
    UpdateHPBar();
	hPBar_->Update();
}
//描画
void Ui::Draw()
{
	hPBar_->Draw();
}
//デバック
void Ui::Debug()
{
#ifdef USE_IMGUI
    // HPバーの位置・サイズ取得
    Vector2 position = hPBar_->GetPosition();
    Vector2 size = hPBar_->GetSize();

    // ImGuiで位置・サイズを編集
    if (ImGui::Begin("HPBar Debug")) {
        ImGui::DragFloat2("Position", &position.x, 1.0f);
        ImGui::DragFloat2("Size", &size.x, 1.0f);

        // 変更を反映
        hPBar_->SetPosition(position);
        hPBar_->SetSize(size);
    }
    ImGui::End();
#endif
}
//HPバー更新
void Ui::UpdateHPBar() {

    // 現在のHPを取得
    float currentHP = static_cast<float>(player_->GetHP());

    // HPの割合
    constexpr float maxHPValue = 100.0f;
    float hpRatio = std::clamp(currentHP / maxHPValue, 0.0f, 1.0f);

    // 最大サイズ
    Vector2 maxSize = { 200.0f, 20.0f };

    // HP割合に応じて横幅を調整
    hPBar_->SetSize({ maxSize.x * hpRatio, maxSize.y });
}