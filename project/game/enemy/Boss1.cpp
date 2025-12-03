#include "Boss1.h"
#include "ImGuiManager.h"
#include "MyMath.h"
#include "Player.h"

/// 初期化
void Boss1::Initialize(Object3dBase* object3dBase)
{
    EnemyBase::Initialize(object3dBase);

    object_->SetModel("boss/boss.obj");
    object_->SetLight(false);

    // 初期位置・スケール設定
    transform_.scale = { 1.5f, 1.5f, 1.5f };
    transform_.translate = { 0.0f, 5.0f, 40.0f };

}

/// 更新
void Boss1::Update()
{
    if (isDead_) return;

    // TransformをObject3dに反映
    object_->SetScale(transform_.scale);
    object_->SetRotate(transform_.rotate);
    object_->SetTranslate(transform_.translate);
    object_->Update();
}

/// 描画
void Boss1::Draw()
{
    if (isDead_) return;
    if (object_) object_->Draw();
}

/// デバッグ
void Boss1::Debug()
{
#ifdef USE_IMGUI

#endif
}

/// 当たり判定用OBB
OBB Boss1::GetOBB() const
{
    OBB obb;
    obb.center = transform_.translate;

    Matrix4x4 rotMat = Math::MakeRotateMatrix(transform_.rotate);
    obb.orientations[0] = Math::Normalize({ rotMat.m[0][0], rotMat.m[1][0], rotMat.m[2][0] });
    obb.orientations[1] = Math::Normalize({ rotMat.m[0][1], rotMat.m[1][1], rotMat.m[2][1] });
    obb.orientations[2] = Math::Normalize({ rotMat.m[0][2], rotMat.m[1][2], rotMat.m[2][2] });

    obb.size = (transform_.scale * 5.0f) * 0.5f;
    return obb;
}
