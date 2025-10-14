#include "RailCamera.h"
#include "MyMath.h"

RailCamera::RailCamera() {}

void RailCamera::Initialize() {
    camera_.SetTranslate({ 0, 0, -10 });
}

void RailCamera::SetPlayer(Player* player) {
    player_ = player;
}

void RailCamera::SetPlayerOffset(const Vector3& offset) {
    playerOffset_ = offset;
}

void RailCamera::SetPlatform(Platform* platform) {
    platform_ = platform;
}

void RailCamera::SetPlatformOffset(const Vector3& offset) {
    platformOffset_ = offset;
}

void RailCamera::SetSpeed(float speed) {
    speed_ = speed;
}

Camera* RailCamera::GetCamera() {
    return &camera_;
}

void RailCamera::Update() {
    //+Z方向に進める
    Vector3 camPos = camera_.GetTranslate();
    camPos.z += speed_;
    camera_.SetTranslate(camPos);

    camera_.Update();

    //プレイヤーをカメラに追従
    if (player_) {
        Vector3 playerPos = player_->GetTranslate();
        //Z座標をカメラ基準
        playerPos.z = camPos.z + playerOffset_.z;

        player_->SetTranslate(playerPos);
    }

    if (platform_) {
        Vector3 platPos = platformOffset_;
        platPos.z += camPos.z;
        platform_->SetTransform(platPos);
    }
}