#include "TitleObject.h"
#include "ImGuiManager.h"

// タイトル画面用オブジェクトの初期化処理
void TitleObject::Initialize() {
    // タイトルのオブジェクト生成・初期化
    title_ = std::make_unique<Object3d>();
    title_->Initialize(Object3dBase::GetInstance());
    title_->SetModel("title.obj");
    title_->SetScale({ 1.2f, 1.2f, 1.2f });
    title_->SetTranslate({ -1.2f, 0.8f, 1.0f });

    // pushspaceのオブジェクト生成・初期化
    push_space_ = std::make_unique<Object3d>();
    push_space_->Initialize(Object3dBase::GetInstance());
    push_space_->SetModel("pushspace.obj");
    push_space_->SetScale({ 0.5f, 0.5f, 0.5f });
    push_space_->SetTranslate({ 1.1f, -1.5f, 1.0f });

    // プレイヤーオブジェクト（外見のみ）生成・初期化
    player_obj_ = std::make_unique<Object3d>();
    player_obj_->Initialize(Object3dBase::GetInstance());
    player_obj_->SetModel("player/player.obj");
    player_obj_transform_.scale = { 0.5f, 0.5f, 0.5f };
    player_obj_transform_.rotate = { 0.0f, -0.5f, 0.0f };
    player_obj_transform_.translate = { -1.3f, -0.5f, -4.8f };
}

// 毎フレームの更新処理
void TitleObject::Update() {
    // タイトルオブジェクトの更新
    title_->Update();

    // pushspaceの点滅演出（アルファ値を周期的に変化させる）
    alpha_timer_ += kDeltaTime;
    alpha_ = (sinf(alpha_timer_ * 3.0f) * 0.5f) + 0.5f;
    push_space_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
    push_space_->Update();

    // プレイヤーオブジェクトの演出（回転・ジャンプ処理）
    PlayerObjDirection();
}

// 描画処理
void TitleObject::Draw() {
    title_->Draw();
    push_space_->Draw();
    player_obj_->Draw();
}

// プレイヤーオブジェクトの演出（回転・ジャンプ処理）
void TitleObject::PlayerObjDirection() {
    // プレイヤーオブジェクトの回転
    player_obj_transform_.rotate.y += kRotateSpeed * kDeltaTime;

    // ジャンプタイマー更新
    jump_timer_ += 1.0f;

    // ジャンプ開始判定
    if (!is_jumping_ && jump_timer_ >= jump_interval_) {
        jump_velocity_ = jump_power_;
        is_jumping_ = true;
        jump_count_ = 1;
        jump_timer_ = 0.0f;
    }

    // ジャンプ中の処理
    if (is_jumping_) {
        player_obj_transform_.translate.y += jump_velocity_;
        jump_velocity_ += gravity_;

        // 2段目ジャンプ判定
        if (jump_count_ == 1 && jump_velocity_ < 0.0f) {
            jump_velocity_ = jump_power_ * 1.2f;
            jump_count_ = 2;
        }

        // 2段目ジャンプ時の回転
        if (jump_count_ == 2) {
            player_obj_transform_.rotate.x += jump_rotate_speed_ * kDeltaTime;
        }

        // 着地判定
        if (player_obj_transform_.translate.y <= ground_y_) {
            player_obj_transform_.translate.y = ground_y_;
            jump_velocity_ = 0.0f;
            is_jumping_ = false;
            jump_count_ = 0;
            player_obj_transform_.rotate.x = 0.0f;
        }
    }

    // Transform情報をObject3dへ反映
    player_obj_->SetScale(player_obj_transform_.scale);
    player_obj_->SetRotate(player_obj_transform_.rotate);
    player_obj_->SetTranslate(player_obj_transform_.translate);
    player_obj_->Update();
}

// デバッグ表示（ImGuiによるパラメータ調整）
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

    // pushspaceオブジェクトのパラメータ調整
    static float push_scale[3] = { 0.5f, 0.5f, 0.5f };
    static float push_rotate[3] = { 0.0f, 0.0f, 0.0f };
    static float push_translate[3] = { 1.1f, -1.5f, 1.0f };

    ImGui::Text("PushSpace");
    if (ImGui::DragFloat3("Push Scale", push_scale, 0.01f)) {
        push_space_->SetScale({ push_scale[0], push_scale[1], push_scale[2] });
    }
    if (ImGui::DragFloat3("Push Rotate", push_rotate, 0.5f)) {
        push_space_->SetRotate({ push_rotate[0], push_rotate[1], push_rotate[2] });
    }
    if (ImGui::DragFloat3("Push Translate", push_translate, 0.01f)) {
        push_space_->SetTranslate({ push_translate[0], push_translate[1], push_translate[2] });
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

    ImGui::End();
#endif
}
