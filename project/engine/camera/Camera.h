#pragma once
#include "Struct.h"
#include "WindowsAPI.h"

namespace MyEngine {

	// カメラクラス
	class Camera {
	public:
		/// <summary>
		/// デフォルトコンストラクタ。
		/// </summary>
		Camera();

		/// <summary>
		/// 毎フレーム更新する。
		/// </summary>
		void Update();

		/// <summary>回転を設定する。</summary>
		void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
		/// <summary>平行移動（座標）を設定する。</summary>
		void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
		/// <summary>垂直方向（y軸）視野角を設定する。</summary>
		void SetFovY(float& fov_y) { fov_y_ = fov_y; }
		/// <summary>アスペクト比を設定する（幅 / 高さ）。</summary>
		void SetAspectRatio(float& aspect_ratio) { aspect_ratio_ = aspect_ratio; }
		/// <summary>ニアクリップ距離を設定する。</summary>
		void SetNearClip(float& near_clip) { near_clip_ = near_clip; }
		/// <summary>ファークリップ距離を設定する。</summary>
		void SetFarClip(float& far_clip) { far_clip_ = far_clip; }

		/// <summary>ワールド行列を取得する。</summary>
		const Matrix4x4& GetWorldMatrix() const { return world_matrix_; }
		/// <summary>ビュー行列を取得する。</summary>
		const Matrix4x4& GetViewMatrix() const { return view_matrix_; }
		/// <summary>プロジェクション行列を取得する。</summary>
		const Matrix4x4& GetProjectionMatrix() const { return projection_matrix_; }
		/// <summary>ビュー×プロジェクション行列を取得する。</summary>
		const Matrix4x4& GetViewProjectionMatrix() const { return view_projection_matrix_; }
		/// <summary>回転を取得する。</summary>
		const Vector3& GetRotate() const { return transform_.rotate; }
		/// <summary>座標を取得する。</summary>
		const Vector3& GetTranslate() const { return transform_.translate; }

	private:
		Transform transform_;
		Matrix4x4 world_matrix_;
		Matrix4x4 view_matrix_;
		Matrix4x4 projection_matrix_;
		Matrix4x4 view_projection_matrix_;
		// 垂直方向視野角
		float fov_y_;
		// アスペクト比
		float aspect_ratio_;
		// ニアクリップ距離
		float near_clip_;
		// ファークリップ距離
		float far_clip_;
	};
}