#include "Camera.h"
#include "Math.h"
#include "ImGuiManager.h"

//デフォルトコンストラクタ
Camera::Camera()
	:transform({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
	, FovY(0.45f)
	, aspectRatio((WindowsAPI::kClientWitdh) / float(WindowsAPI::kClientHeight))
	, nearClip(0.1f)
	, farClip(100.0f)
	, worldMatrix(Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate))
	, viewMatrix(Math::Inverse(worldMatrix))
	, projectionMatrix(Math::MakePerspectiveFovMatrix(FovY, aspectRatio, nearClip, farClip))
	, viewProjectionMatrix(Math::Multiply(viewMatrix, projectionMatrix))
{}

//更新
void Camera::Update() {
	//ワールド行列
	worldMatrix = Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//ビュー行列
	viewMatrix = Math::Inverse(worldMatrix);
	//プロジェクション行列
	projectionMatrix = Math::MakePerspectiveFovMatrix(FovY, aspectRatio, nearClip, farClip);
	//合成行列
	viewProjectionMatrix = Math::Multiply(viewMatrix, projectionMatrix);
}

//デバック
void Camera::DebugUpdate() {
#ifdef USE_IMGUI
	ImGui::Begin("Settings");
	ImGui::DragFloat3("CameraTranslate", &transform.translate.x, 0.01f, -10.0f, 10.0f);
	ImGui::End();
#endif
}