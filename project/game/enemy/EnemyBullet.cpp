#include "EnemyBullet.h"
#include "MyMath.h"

// 弾の初期化処理
void EnemyBullet::Initialize(MyEngine::Object3dBase* object3d_base) {
    object3d_base_ = object3d_base;
    object_ = std::make_unique< MyEngine::Object3d>();
    object_->Initialize(object3d_base_);
    object_->SetModel("enemy/enemybullet.obj");
    object_->SetScale({ 1.0f, 1.0f, 1.0f });
    transform_.translate = { 0.0f, 0.0f, 0.0f };
}

// 毎フレームの更新処理
void EnemyBullet::Update() {
    // 速度ベクトル分だけ座標を移動
    transform_.translate = Math::Add(transform_.translate, velocity_);

    // 寿命タイマー減算・0以下で消滅フラグ
    if (--death_timer_ <= 0) {
        is_dead_ = true;
    }

    // オブジェクトの座標を更新
    object_->SetTranslate(transform_.translate);
    object_->Update();
}

// 弾の描画処理
void EnemyBullet::Draw() {
    object_->Draw();
}

// 衝突時の処理（消滅フラグを立てる）
void EnemyBullet::OnCollision() {
    is_dead_ = true;
}

// OBB（当たり判定用の回転付きボックス）取得
OBB EnemyBullet::GetOBB() const {
    OBB obb;
    obb.center = transform_.translate;

    // 回転行列から各軸ベクトルを算出・正規化
    Matrix4x4 rotMat = Math::MakeRotateMatrix(transform_.rotate);
    obb.orientations[0] = Math::Normalize({ rotMat.m[0][0], rotMat.m[1][0], rotMat.m[2][0] }); // X軸
    obb.orientations[1] = Math::Normalize({ rotMat.m[0][1], rotMat.m[1][1], rotMat.m[2][1] }); // Y軸
    obb.orientations[2] = Math::Normalize({ rotMat.m[0][2], rotMat.m[1][2], rotMat.m[2][2] }); // Z軸

    // スケールと寸法からサイズ算出
    obb.size = (transform_.scale * dimensions_) * 0.5f;

    return obb;
}
