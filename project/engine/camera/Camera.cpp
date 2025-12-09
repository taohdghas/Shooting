#include "Camera.h"
#include "MyMath.h"

namespace MyEngine {

	Camera::Camera() {
		// Transformの初期化
		transform_.scale = { 1.0f, 1.0f, 1.0f };
		transform_.rotate = { 0.0f, 0.0f, 0.0f };
		transform_.translate = { 0.0f, 0.0f, 0.0f };

		// 視野角・アスペクト比・クリップ距離の初期化
		fov_y_ = 0.45f;
		aspect_ratio_ = 16.0f / 9.0f; // 仮値。必要に応じてWindowAPIから取得
		near_clip_ = 0.1f;
		far_clip_ = 100.0f;

		// 行列の初期化
		world_matrix_ = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		view_matrix_ = Math::Inverse(world_matrix_);
		projection_matrix_ = Math::MakePerspectiveFovMatrix(fov_y_, aspect_ratio_, near_clip_, far_clip_);
		view_projection_matrix_ = Math::Multiply(view_matrix_, projection_matrix_);
	}

	void Camera::Update() {
		// worldMatrixの更新
		world_matrix_ = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		// viewMatrixの更新
		view_matrix_ = Math::Inverse(world_matrix_);
		// projectionMatrixの更新
		projection_matrix_ = Math::MakePerspectiveFovMatrix(fov_y_, aspect_ratio_, near_clip_, far_clip_);
		// viewProjectionMatrixの更新
		view_projection_matrix_ = Math::Multiply(view_matrix_, projection_matrix_);
	}
}