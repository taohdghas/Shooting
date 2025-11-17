#include "RailCamera.h"
#include "Player.h"
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
    playerOffset_ = offset;
}

// プラットフォームの参照をセット
void RailCamera::SetPlatform(Platform* platform) {
    platform_ = platform;
}

// プラットフォームのカメラからのオフセットをセット
void RailCamera::SetPlatformOffset(const Vector3& offset) {
    platformOffset_ = offset;
}

// カメラの進行速度をセット
void RailCamera::SetSpeed(float speed) {
    speed_ = speed;
}

void RailCamera::EnableFollow(bool enable) {
    followEnabled_ = enable;
}

// カメラのポインタを取得
Camera* RailCamera::GetCamera() {
    return &camera_;
}

// 毎フレームの更新処理
void RailCamera::Update() {
    if (followEnabled_) {
        //カメラ移動（Z方向）
        Vector3 camPos = camera_.GetTranslate();
        camPos.z += speed_;
        camera_.SetTranslate(camPos);

        //プレイヤー追従
        if (player_) {
            Vector3 playerPos = player_->GetTranslate();
            playerPos.z = camPos.z + playerOffset_.z;
            player_->SetTranslate(playerPos);
        }

        //プラットフォーム追従
        if (platform_) {
            Vector3 platPos = platformOffset_;
            platPos.z += camPos.z;
            platform_->SetTransform(platPos);
        }
    }

    //Camera更新
   camera_.Update();
}