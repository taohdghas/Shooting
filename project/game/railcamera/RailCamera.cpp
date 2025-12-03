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
   // Vector3 camPos = camera_.GetTranslate();
    //camPos.x += (targetPos.x - camPos.x) * speed_;
    //camPos.y += (targetPos.y - camPos.y) * speed_;
    //camPos.z += (targetPos.z - camPos.z) * speed_;

    camera_.SetTranslate(targetPos);
    camera_.Update();
}
//デバッグ
void RailCamera::Debug() {
#ifdef USE_IMGUI
    const Vector3& camPos = camera_.GetTranslate();
    ImGui::Text("Position: X=%.2f Y=%.2f Z=%.2f", camPos.x, camPos.y, camPos.z);
#endif
}