#include "Ui.h"
#include "SpriteBase.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <algorithm>

//初期化
void Ui::Initialize()
{   //HPバー作成
    CreateSprite(SpriteType::HpBar, "resources/ui/hpbar.png", { 50,50 }, { 200,20 });
	//ポーズ文字作成
    CreateSprite(SpriteType::Pause,"resources/ui/pause.png", { 1000,20 }, { 300,60 });
    //リトライ文字作成
	//CreateSprite(SpriteType::Retry, "resources/ui/retry.png", { 640,420 }, { 400,100 });
	//タイトルへ戻る文字作成
	//CreateSprite(SpriteType::BackTitle, "resources/ui/back_title.png", { 640,540 }, { 400,100 });
	//操作説明画面作成
    CreateSprite(SpriteType::OperationGuide,"resources/ui/operation.png", { 640,300 }, { 500,500 }, { 0.5f,0.5f });
	//各キー説明作成
    CreateSprite(SpriteType::KeyA, "resources/ui/keyboard_a.png", { 440,500 }, { 50,50 });
	//Dキー
    CreateSprite(SpriteType::KeyD, "resources/ui/keyboard_d.png", { 540,500 }, { 50,50 });
	//Fキー
    CreateSprite(SpriteType::KeyF,"resources/ui/keyboard_f.png", { 640,500 }, { 50,50 });
	//Wキー
    CreateSprite(SpriteType::KeyW, "resources/ui/keyboard_w.png", { 490,450 }, { 50,50 });
	//マウス左
    CreateSprite(SpriteType::MouseLeft, "resources/ui/mouse_left.png", { 740,500 }, { 50,50 });
	//マウス移動
    CreateSprite(SpriteType::MouseMove,"resources/ui/mouse_move.png", { 840,500 }, { 50,50 });
}
//更新
void Ui::Update()
{
	//HPバー更新
    UpdateHPBar();
	//操作説明画面更新
    UpdatePauseGuide();

	//スプライト更新
    for (auto& [type, sprite] : sprites_) {
        sprite->Update();
    }
}
//描画
void Ui::Draw()
{
	//HPバー表示
    Get(SpriteType::HpBar)->Draw();
	//ポーズ文字表示
    Get(SpriteType::Pause)->Draw();
	//ポーズ画面表示
    if (pause_scale_ > 0.01f) {
       // Get(SpriteType::Retry)->Draw();
		//Get(SpriteType::BackTitle)->Draw();
        Get(SpriteType::OperationGuide)->Draw();
    }
}
//デバック
void Ui::Debug()
{
#ifdef USE_IMGUI
    if (ImGui::Begin("UI Debug")) {

        for (auto& [type, sprite] : sprites_) {

            std::string name = std::to_string(static_cast<int>(type));
            if (ImGui::TreeNode(name.c_str())) {

                Vector2 pos = sprite->GetPosition();
                Vector2 size = sprite->GetSize();
                float rot = sprite->GetRotation();

                ImGui::DragFloat2("Position", &pos.x, 1.0f);
                ImGui::DragFloat2("Size", &size.x, 1.0f);
                ImGui::DragFloat("Rotation", &rot, 0.01f);

                sprite->SetPosition(pos);
                sprite->SetSize(size);
                sprite->SetRotation(rot);

                ImGui::TreePop();
            }
        }
    }
    ImGui::End();
#endif
}
//HPバー更新
void Ui::UpdateHPBar()
{
    if (!player_) return;

    float currentHP = static_cast<float>(player_->GetHP());
    constexpr float maxHP = 100.0f;

    float ratio = std::clamp(currentHP / maxHP, 0.0f, 1.0f);

    Vector2 maxSize = { 200.0f, 20.0f };
    Get(SpriteType::HpBar)->SetSize({ maxSize.x * ratio, maxSize.y });
}
//操作説明画面処理
void Ui::UpdatePauseGuide()
{
    const float speed = 0.07f;
    //スケール更新
    if (is_show_pause_) {
        pause_scale_ += speed;
    } else {
        pause_scale_ -= speed;
    }
    pause_scale_ = std::clamp(pause_scale_, 0.0f, 1.0f);

    float eased = static_cast<float>(Math::easeOutQuad(pause_scale_));

    //Get(SpriteType::Retry)->SetSize( Math::MultiplyScalar(retry_base_size, eased));

    //Get(SpriteType::BackTitle)->SetSize(Math::MultiplyScalar(back_title_base_size, eased));

    Get(SpriteType::OperationGuide)->SetSize(Math::MultiplyScalar(operation_base_size, eased));
}
//スプライト作成
void Ui::CreateSprite(SpriteType type,
    const char* path,
    const Vector2& pos,
    const Vector2& size,
    const Vector2& anchor)
{
    auto sprite = std::make_unique<MyEngine::Sprite>();
    sprite->Initialize(MyEngine::SpriteBase::GetInstance(), path);
    sprite->SetPosition(pos);
    sprite->SetSize(size);
    sprite->SetAnchorPoint(anchor);

    sprites_[type] = std::move(sprite);
}
///スプライト取得
MyEngine::Sprite* Ui::Get(SpriteType type)
{
    auto it = sprites_.find(type);
    if (it == sprites_.end()) return nullptr;
    return it->second.get();
}