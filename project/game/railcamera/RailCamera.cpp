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

    if (platform_) {
        Vector3 platPos = platformOffset_;
        platPos.z += camPos.z;
        platform_->SetTranslate(platPos);
    }
}