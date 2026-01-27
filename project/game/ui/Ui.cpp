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
	//回避ゲージ作成
    CreateSprite(SpriteType::DodgeGauge, "resources/ui/dodgecooldownbar.png", { 50,80 }, { 200,20 });
	//回避ゲージフラッシュ作成
    CreateSprite(SpriteType::DodgeGaugeFlash, "resources/ui/dodgecooldownbar_flash.png", { 50,80 }, { 200,20 });
	//ポーズ文字作成
    CreateSprite(SpriteType::Pause,"resources/ui/pause.png", { 1000,20 }, { 300,60 });
    //リトライ文字作成
    CreateSprite(SpriteType::Retry, "resources/ui/retry.png", { 485,620 }, { 180,100 }, { 0.5f,0.5f });
    //タイトルへ戻る文字作成
    CreateSprite(SpriteType::BackTitle, "resources/ui/back_title.png", { 805,620 }, { 180,100 }, { 0.5f,0.5f });
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
	//ポーズボタンホバー更新
    UpdatePauseButtonHover();
	//ポーズメニュークリック更新
    UpdatePauseClick();
	//回避ゲージ更新
    UpdateDodgeGauge();

	//スプライト更新
    for (auto& [type, sprite] : sprites_) {
        sprite->Update();
    }
}
//描画
void Ui::Draw()
{
	//HPバー描画
    Get(SpriteType::HpBar)->Draw();
    //通常ゲージ
    if (!is_dodge_flash_visible_) {
        Get(SpriteType::DodgeGaugeFlash)->Draw();
    }
    //強調ゲージ
    else {
        Get(SpriteType::DodgeGauge)->Draw();
    }
	//ポーズ文字描画
    Get(SpriteType::Pause)->Draw();
	//ポーズメニュー描画
    if (pause_scale_ > 0.01f) {
        Get(SpriteType::Retry)->Draw();
        Get(SpriteType::BackTitle)->Draw();
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
	//現在のHP取得
    float currentHP = static_cast<float>(player_->GetHP());
    constexpr float maxHP = 100.0f;
	//HP比率計算
    float ratio = std::clamp(currentHP / maxHP, 0.0f, 1.0f);

    Vector2 maxSize = { 200.0f, 20.0f };
	//サイズ更新
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
	//イージング適用
    float eased = static_cast<float>(Math::easeOutQuad(pause_scale_));

    Get(SpriteType::Retry)->SetSize( Math::MultiplyScalar(retry_base_size, eased));

    Get(SpriteType::BackTitle)->SetSize(Math::MultiplyScalar(back_title_base_size, eased));

    Get(SpriteType::OperationGuide)->SetSize(Math::MultiplyScalar(operation_base_size, eased));
}

//ポーズボタンホバー処理
void Ui::UpdatePauseButtonHover()
{
    if (pause_scale_ <= 0.01f) return; // ポーズ画面が出てない

    hover_timer_ += 1.0f / 60.0f;

    float animationScale = 1.0f + sinf(hover_timer_ * 5.0f) * 0.1f;

    float eased = static_cast<float>(Math::easeOutQuad(pause_scale_));

    Vector2 retrySize = Math::MultiplyScalar(retry_base_size, eased);
    Vector2 backSize = Math::MultiplyScalar(back_title_base_size, eased);

    // 通常サイズに戻す
    Get(SpriteType::Retry)->SetSize(retrySize);
    Get(SpriteType::BackTitle)->SetSize(backSize);

	// リトライボタン
    if (IsMouseOnSprite(Get(SpriteType::Retry))) {
        Get(SpriteType::Retry)->SetSize(Math::MultiplyScalar(retrySize, animationScale));
    }
	// タイトルへ戻るボタン
    if (IsMouseOnSprite(Get(SpriteType::BackTitle))) {
        Get(SpriteType::BackTitle)->SetSize(Math::MultiplyScalar(backSize, animationScale));
    }
}
//ポーズメニュークリック処理
void Ui::UpdatePauseClick()
{
    if (!is_show_pause_) return;
    if (pause_scale_ < 0.95f) return; // 出現中は無効（誤爆防止）

    auto input = MyEngine::Input::GetInstance();

    if (!input->IsMouseLeftTriggered()) return;

    if (IsMouseOnSprite(Get(SpriteType::Retry))) {
        pause_result_ = PauseResult::Retry;
    } else if (IsMouseOnSprite(Get(SpriteType::BackTitle))) {
        pause_result_ = PauseResult::BackTitle;
    }
}
//回避クールタイムバー更新
void Ui::UpdateDodgeGauge()
{
    if (!player_) return;

    float ratio = player_->GetDodgeCooldownRatio();
    constexpr Vector2 maxSize = { 200.0f, 20.0f };

    Vector2 size = { maxSize.x * ratio, maxSize.y };

    Get(SpriteType::DodgeGauge)->SetSize(size);
    Get(SpriteType::DodgeGaugeFlash)->SetSize(size);

    //点滅制御
    if (ratio < 1.0f) {
        //クールタイム中交互にスプライト表示
        dodge_flash_timer_ += 1.0f / 60.0f;
		//一定時間ごとに表示非表示切り替え
        if (dodge_flash_timer_ > 0.2f) {  
            dodge_flash_timer_ = 0.0f;
            is_dodge_flash_visible_ = !is_dodge_flash_visible_;
        }
    } else {
        // 満タン → 強調表示固定
        is_dodge_flash_visible_ = true;
    }
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

//スプライト上にマウスが乗っているか
bool Ui::IsMouseOnSprite(MyEngine::Sprite* sprite)
{
    if (!sprite) return false;

    POINT mouse = MyEngine::Input::GetInstance()->GetMousePosition();

    Vector2 pos = sprite->GetPosition();
    Vector2 size = sprite->GetSize();
    Vector2 anchor = sprite->GetAnchorPoint();

    Vector2 leftTop;
    leftTop.x = pos.x - size.x * anchor.x;
    leftTop.y = pos.y - size.y * anchor.y;

    Vector2 rightBottom;
    rightBottom.x = leftTop.x + size.x;
    rightBottom.y = leftTop.y + size.y;

    if (mouse.x >= leftTop.x && mouse.x <= rightBottom.x &&
        mouse.y >= leftTop.y && mouse.y <= rightBottom.y) {
        return true;
    }

    return false;
}

///スプライト取得
MyEngine::Sprite* Ui::Get(SpriteType type)
{
    auto it = sprites_.find(type);
    if (it == sprites_.end()) return nullptr;
    return it->second.get();
}