#include "Camera.h"
#include "MyMath.h"

// デフォルトコンストラクタ
Camera::Camera()
    : transform({ {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} }) // scale, rotate, translate
    , FovY(0.45f) // 垂直方向の視野角
    , aspectRatio((WindowsAPI::kClientWidth) / float(WindowsAPI::kClientHeight)) // 画面のアスペクト比
    , nearClip(0.1f) // ニアクリップ距離
    , farClip(100.0f) // ファークリップ距離
    , worldMatrix(Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate)) // ワールド変換行列
    , viewMatrix(Math::Inverse(worldMatrix)) // ビュー行列はワールド行列の逆行列
    , projectionMatrix(Math::MakePerspectiveFovMatrix(FovY, aspectRatio, nearClip, farClip)) // 射影行列
    , viewProjectionMatrix(Math::Multiply(viewMatrix, projectionMatrix)) // ビュー×射影行列
{
}

// 更新処理
void Camera::Update() {
    // ワールド行列を再計算
    worldMatrix = Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

    // ビュー行列はワールド行列の逆行列
    viewMatrix = Math::Inverse(worldMatrix);

    // プロジェクション行列を再計算（Fovやアスペクト比の変更対応）
    projectionMatrix = Math::MakePerspectiveFovMatrix(FovY, aspectRatio, nearClip, farClip);

    // ビュー行列とプロジェクション行列を掛け合わせて最終的な変換行列を作成
    viewProjectionMatrix = Math::Multiply(viewMatrix, projectionMatrix);
}
