#include "RailCamera.h"
#include "MyMath.h"

RailCamera::RailCamera() {}

// カメラの初期化処理
void RailCamera::Initialize() {
    camera_.SetTranslate({ 0, 0, -10 });
}

// プレイヤーの参照をセット
void RailCamera::SetPlayer(Player* player) {
    player_ = player;
}

// プレイヤーのカメラからのオフセットをセット
void RailCamera::SetPlayerOffset(const Vector3& offset) {
    player_offset_ = offset;
}

// プラットフォームの参照をセット
void RailCamera::SetPlatform(Platform* platform) {
    platform_ = platform;
}

// プラットフォームのカメラからのオフセットをセット
void RailCamera::SetPlatformOffset(const Vector3& offset) {
    platform_offset_ = offset;
}

// カメラの進行速度をセット
void RailCamera::SetSpeed(float speed) {
    speed_ = speed;
}

// カメラのポインタを取得
Camera* RailCamera::GetCamera() {
    return &camera_;
}

// 毎フレームの更新処理
void RailCamera::Update() {
    // カメラを+Z方向に移動
    Vector3 camPos = camera_.GetTranslate();
    camPos.z += speed_;
    camera_.SetTranslate(camPos);

    camera_.Update();

    // プレイヤーをカメラに追従させる（Z座標のみカメラ基準で調整）
    if (player_) {
        Vector3 playerPos = player_->GetTranslate();
        playerPos.z = camPos.z + player_offset_.z;
        player_->SetTranslate(playerPos);
    }

    // プラットフォームをカメラに追従させる（Z座標のみカメラ基準で調整）
    if (platform_) {
        Vector3 platPos = platform_offset_;
        platPos.z += camPos.z;
        platform_->SetTransform(platPos);
    }
}