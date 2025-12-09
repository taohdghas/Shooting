#include "RailCamera.h"
#include "MyMath.h"

RailCamera::RailCamera() {}

// カメラの初期化処理
void RailCamera::Initialize() {
    camera_.SetTranslate({ 0, 0, -10 });
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
        platform_->SetTranslate(platPos);
    }
}