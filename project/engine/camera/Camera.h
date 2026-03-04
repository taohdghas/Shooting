#pragma once
#include "Struct.h"
#include "WindowsAPI.h"

namespace MyEngine {

	// カメラクラス
	/// <summary>
	///3D空間におけるカメラの位置・回転・投影パラメータの管理
	///ワールド・ビュー・プロジェクション・ビュー×プロジェクション各行列の生成・保持
	///毎フレームのカメラ行列更新処理
	///外部からのカメラパラメータ設定・取得インターフェースの提供
	/// </summary>
	class Camera {
	public:
		/// <summary>
        /// デフォルトコンストラクタ
        /// <returns>なし</returns>
        /// </summary>
		Camera();

		/// <summary>
		/// 毎フレーム更新
		/// <returns>なし</returns>
		/// </summary>
		void Update();

		/// <summary>回転を設定
		/// <param name="rotate">回転値（Vector3）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
		/// <summary>平行移動を設定
		/// <param name="translate">座標値（Vector3）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
		/// <summary>垂直方向視野角を設定
		/// <param name="fov_y">視野角（float）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetFovY(float fov_y) { fov_y_ = fov_y; }
		/// <summary>アスペクト比を設定する
		/// <param name="aspect_ratio">アスペクト比（float）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; }
		/// <summary>ニアクリップ距離を設定
		/// <param name="near_clip">ニアクリップ距離（float）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetNearClip(float near_clip) { near_clip_ = near_clip; }
		/// <summary>ファークリップ距離を設定
		/// <param name="far_clip">ファークリップ距離（float）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetFarClip(float far_clip) { far_clip_ = far_clip; }

		/// <summary>ワールド行列を取得
		/// <returns>ワールド行列（const Matrix4x4&）</returns>
		/// </summary>
		const Matrix4x4& GetWorldMatrix() const { return world_matrix_; }
		/// <summary>ビュー行列を取得
		/// <returns>ビュー行列（const Matrix4x4&）</returns>
		/// </summary>
		const Matrix4x4& GetViewMatrix() const { return view_matrix_; }
		/// <summary>プロジェクション行列を取得
		/// <returns>プロジェクション行列（const Matrix4x4&）</returns>
		/// </summary>
		const Matrix4x4& GetProjectionMatrix() const { return projection_matrix_; }
		/// <summary>ビュー×プロジェクション行列を取得
		/// <returns>ビュー×プロジェクション行列（const Matrix4x4&）</returns>
		/// </summary>
		const Matrix4x4& GetViewProjectionMatrix() const { return view_projection_matrix_; }
		/// <summary>回転を取得
		/// <returns>回転値（const Vector3&）</returns>
		/// </summary>
		const Vector3& GetRotate() const { return transform_.rotate; }
		/// <summary>座標を取得
		/// <returns>座標値（const Vector3&）</returns>
		/// </summary>
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