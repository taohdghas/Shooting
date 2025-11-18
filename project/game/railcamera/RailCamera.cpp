#include "RailCamera.h"
#include "Player.h"
#include "MyMath.h"
#include "ImGuiManager.h"

RailCamera::RailCamera() {}

//カメラの初期化処理
void RailCamera::Initialize() {
    camera_.SetTranslate({ 0, 0, -10 });
}
//毎フレームの更新処理
void RailCamera::Update() {
    if (!followEnabled_ || !player_) return;

    // 追従ターゲット位置
    Vector3 targetPos = player_->GetTranslate() + playerOffset_;
    if (platform_) targetPos += platformOffset_;

    // 補間で追従
    Vector3 camPos = camera_.GetTranslate();
    camPos.x += (targetPos.x - camPos.x) * speed_;
    camPos.y += (targetPos.y - camPos.y) * speed_;
    camPos.z += (targetPos.z - camPos.z) * speed_;

    camera_.SetTranslate(camPos);
    camera_.Update();
}
//デバッグ
void RailCamera::Debug() {
#ifdef USE_IMGUI
    const Vector3& camPos = camera_.GetTranslate();
    ImGui::Text("Position: X=%.2f Y=%.2f Z=%.2f", camPos.x, camPos.y, camPos.z);
#endif
}

// カメラの座標（平行移動）を設定
void RailCamera::SetTranslate(const Vector3& pos) {
    camera_.SetTranslate(pos);
}
// カメラの回転を設定
void RailCamera::SetRotate(const Vector3& rot) {
    camera_.SetRotate(rot);
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

