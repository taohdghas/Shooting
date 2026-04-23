#include "TitleObject.h"
#include "SpriteBase.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <algorithm>

// タイトル画面用オブジェクトの初期化処理
void TitleObject::Initialize() {
    // タイトルのオブジェクト生成・初期化
    title_ = std::make_unique< MyEngine::Object3d>();
    title_->Initialize(MyEngine::Object3dBase::GetInstance());
    title_->SetModel("titleobject/title.obj");
    title_->SetScale({ 0.74f, 1.2f, 1.2f });
    title_->SetTranslate({ -1.3f, 0.8f, 1.0f });

    // プレイヤーオブジェクト（外見のみ）生成・初期化
    player_obj_ = std::make_unique< MyEngine::Object3d>();
    player_obj_->Initialize(MyEngine::Object3dBase::GetInstance());
    player_obj_->SetModel("player/player.obj");
    player_obj_transform_.scale = { 0.5f, 0.5f, 0.5f };
    player_obj_transform_.rotate = { 0.0f, 2.5f, 0.0f };
    player_obj_transform_.translate = { -1.3f, -0.5f, -4.8f };
	// スタートスプライト生成・初期化
    menu_start_ = std::make_unique<MyEngine::Sprite>();
    menu_start_->Initialize(MyEngine::SpriteBase::GetInstance(),"resources/ui/start.png");
	menu_start_->SetAnchorPoint({ 0.5f, 0.5f });
    menu_start_->SetPosition({ 765, 380 });
	menu_start_->SetSize({150,100});
	// 操作説明項目スプライト
    menu_howto_ = std::make_unique<MyEngine::Sprite>();
    menu_howto_->Initialize(MyEngine::SpriteBase::GetInstance(),"resources/ui/howto.png");
	menu_howto_->SetAnchorPoint({ 0.5f, 0.5f });
    menu_howto_->SetPosition({ 782, 505 });
	menu_howto_->SetSize({ 200,80 });
	// 終了項目スプライト
    menu_exit_ = std::make_unique<MyEngine::Sprite>();
    menu_exit_->Initialize(MyEngine::SpriteBase::GetInstance(),"resources/ui/exit.png");
	menu_exit_->SetAnchorPoint({ 0.5f, 0.5f });
    menu_exit_->SetPosition({ 752, 607 });
	menu_exit_->SetSize({ 150,80 });
	
    // 操作説明
    howto_sprite_ = std::make_unique<MyEngine::Sprite>();
    howto_sprite_->Initialize(MyEngine::SpriteBase::GetInstance(),"resources/ui/operation.png");
    howto_sprite_->SetAnchorPoint({ 0.5f, 0.5f });
    howto_sprite_->SetPosition({ 640, 360 });
	howto_sprite_->SetSize({ 0, 0 });

	// メニュー項目の元サイズ保存
    start_size_ = { 150,100 };
    howto_size_ = { 200,80 };
    exit_size_ = { 150,80 };
}

// 毎フレームの更新処理
void TitleObject::Update()
{
	// 拡縮用タイマー更新
    scale_timer_ += kDeltaTime;

    title_->Update();
	// プレイヤーオブジェクトの演出処理
    PlayerObjDirection();

    menu_start_->Update();
    menu_howto_->Update();
    menu_exit_->Update();
    howto_sprite_->Update();
	// 操作説明表示の更新
    UpdateHowto();

    auto input = MyEngine::Input::GetInstance();

    // マウスが乗っている項目取得
    int hover = GetMouseHoverIndex();
    select_index_ = hover;

    //拡縮
    MenuSizeUpdate();

    if (!is_show_howto_) {

        // クリック決定
        if (hover != -1 && input->IsMouseLeftTriggered()) {
            switch (hover) {
            case 0: menu_result_ = MenuResult::Start; break;
            case 1: is_show_howto_ = true; break;
            case 2: menu_result_ = MenuResult::Exit; break;
            }
        }

    } else {
        // 操作説明中クリックで戻る
        if (input->IsKeyPressed(DIK_Q)) {
            is_show_howto_ = false;
        }
    }
}

// 描画処理
void TitleObject::Draw() {
    title_->Draw();
    player_obj_->Draw();
}
// メニュー項目スプライトの描画処理
void TitleObject::DrawSprite() {
    menu_start_->Draw();
    menu_howto_->Draw();
    menu_exit_->Draw();

    if (howto_scale_ > 0.0f) {
        howto_sprite_->Draw();
    }
}
// プレイヤーオブジェクトの演出
void TitleObject::PlayerObjDirection() {
    // プレイヤーオブジェクトの回転
    player_obj_transform_.rotate.y += kRotateSpeed * kDeltaTime;

    // Transform情報をObject3dへ反映
    player_obj_->SetScale(player_obj_transform_.scale);
    player_obj_->SetRotate(player_obj_transform_.rotate);
    player_obj_->SetTranslate(player_obj_transform_.translate);
    player_obj_->Update();
}
// メニュー項目スプライトの拡縮処理
void TitleObject::MenuSizeUpdate()
{
    float scale = 1.0f + sinf(scale_timer_ * 5.0f) * 0.1f;

    menu_start_->SetSize(start_size_);
    menu_howto_->SetSize(howto_size_);
    menu_exit_->SetSize(exit_size_);

    if (select_index_ == 0)
        menu_start_->SetSize({ start_size_.x * scale, start_size_.y * scale });

    if (select_index_ == 1)
        menu_howto_->SetSize({ howto_size_.x * scale, howto_size_.y * scale });

    if (select_index_ == 2)
        menu_exit_->SetSize({ exit_size_.x * scale, exit_size_.y * scale });
}
// 操作説明表示の更新
void TitleObject::UpdateHowto() {
    const float speed = 0.07f;

    if (is_show_howto_) {
        howto_scale_ += speed;
    } else {
        howto_scale_ -= speed;
    }

    howto_scale_ = std::clamp(howto_scale_, 0.0f, 1.0f);

    float eased = static_cast<float>(Math::easeOutQuad(howto_scale_));

    Vector2 baseSize = { 600.0f, 600.0f };
    howto_sprite_->SetSize(Math::MultiplyScalar(baseSize, eased));

    howto_sprite_->Update();
}

// デバッグ表示
void TitleObject::Debug() {
#ifdef USE_IMGUI
    ImGui::Begin("TitleObject SetUp");

    // タイトルオブジェクトのパラメータ調整
    static float title_scale[3] = { 1.2f, 1.2f, 1.2f };
    static float title_rotate[3] = { 0.0f, 0.0f, 0.0f };
    static float title_translate[3] = { -1.2f, 0.8f, 1.0f };

    ImGui::Text("Title");
    if (ImGui::DragFloat3("Title Scale", title_scale, 0.01f)) {
        title_->SetScale({ title_scale[0], title_scale[1], title_scale[2] });
    }
    if (ImGui::DragFloat3("Title Rotate", title_rotate, 0.5f)) {
        title_->SetRotate({ title_rotate[0], title_rotate[1], title_rotate[2] });
    }
    if (ImGui::DragFloat3("Title Translate", title_translate, 0.01f)) {
        title_->SetTranslate({ title_translate[0], title_translate[1], title_translate[2] });
    }

    ImGui::Separator();

    // プレイヤーオブジェクトのパラメータ調整
    static float player_scale[3] = { 0.5f, 0.5f, 0.5f };
    static float player_rotate[3] = { 0.0f, 0.0f, 0.0f };
    static float player_translate[3] = { -1.3f, 0.0f, -4.8f };

    ImGui::Text("PlayerObj");
    if (ImGui::DragFloat3("Player Scale", player_scale, 0.01f)) {
        player_obj_transform_.scale = { player_scale[0], player_scale[1], player_scale[2] };
    }
    if (ImGui::DragFloat3("Player Rotate", player_rotate, 0.5f)) {
        player_obj_transform_.rotate = { player_rotate[0], player_rotate[1], player_rotate[2] };
    }
    if (ImGui::DragFloat3("Player Translate", player_translate, 0.01f)) {
        player_obj_transform_.translate = { player_translate[0], player_translate[1], player_translate[2] };
    }

    ImGui::Separator();
    ImGui::Text("Sprites");

    // menu_start_
    auto start_pos = menu_start_->GetPosition();
    auto start_size = menu_start_->GetSize();
    if (ImGui::DragFloat2("Start Pos", &start_pos.x, 1.0f)) {
        menu_start_->SetPosition(start_pos);
    }
    if (ImGui::DragFloat2("Start Size", &start_size.x, 1.0f)) {
        menu_start_->SetSize(start_size);
    }

    // menu_howto_
    auto howto_pos = menu_howto_->GetPosition();
    auto howto_size = menu_howto_->GetSize();
    if (ImGui::DragFloat2("HowTo Pos", &howto_pos.x, 1.0f)) {
        menu_howto_->SetPosition(howto_pos);
    }
    if (ImGui::DragFloat2("HowTo Size", &howto_size.x, 1.0f)) {
        menu_howto_->SetSize(howto_size);
    }

    // menu_exit_
    auto exit_pos = menu_exit_->GetPosition();
    auto exit_size = menu_exit_->GetSize();
    if (ImGui::DragFloat2("Exit Pos", &exit_pos.x, 1.0f)) {
        menu_exit_->SetPosition(exit_pos);
    }
    if (ImGui::DragFloat2("Exit Size", &exit_size.x, 1.0f)) {
        menu_exit_->SetSize(exit_size);
    }

    // howto_sprite_
    auto howto_sprite_pos = howto_sprite_->GetPosition();
    auto howto_sprite_size = howto_sprite_->GetSize();
    if (ImGui::DragFloat2("HowToSprite Pos", &howto_sprite_pos.x, 1.0f)) {
        howto_sprite_->SetPosition(howto_sprite_pos);
    }
    if (ImGui::DragFloat2("HowToSprite Size", &howto_sprite_size.x, 1.0f)) {
        howto_sprite_->SetSize(howto_sprite_size);
    }

    ImGui::End();
#endif
}

// スプライト上にマウスが乗っているか
bool TitleObject::IsMouseOnSprite(MyEngine::Sprite* sprite)
{
    POINT mouse = MyEngine::Input::GetInstance()->GetMousePosition();

    Vector2 pos = sprite->GetPosition();      
    Vector2 size = sprite->GetSize();       
    Vector2 anchor = sprite->GetAnchorPoint();

    // 左上座標をアンカーから逆算
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
// マウスが乗っているメニュー項目のインデックスを取得
int TitleObject::GetMouseHoverIndex()
{
    if (IsMouseOnSprite(menu_start_.get())) return 0;
    if (IsMouseOnSprite(menu_howto_.get())) return 1;
    if (IsMouseOnSprite(menu_exit_.get()))  return 2;

    return -1;
}