#pragma once
#include "Struct.h"
#include "WindowsAPI.h"

// カメラクラス
class Camera {
public:
	/// <summary>
	/// デフォルトコンストラクタ。
	/// - 既定のスケール/回転/位置で Transform を初期化する。
	/// - 視野角は 0.45、アスペクト比は WindowsAPI::kClientWidth / kClientHeight、ニア/ファークリップは 0.1 / 100.0 を設定する。
	/// - world/view/projection/viewProjection 行列を初期値で生成する（Math ユーティリティを使用）。
	/// </summary>
	Camera();

	/// <summary>
	/// 毎フレーム更新する。
	/// - worldMatrix を Transform（スケール/回転/平行移動）から再生成する。
	/// - viewMatrix を worldMatrix の逆行列として計算する。
	/// - projectionMatrix を fov_y_ / aspect_ratio_ / near_clip_ / far_clip_ から計算する。
	/// - view_projection_matrix_ を viewMatrix と projectionMatrix の積として更新する。
	/// </summary>
	void Update();

	/// <summary>回転を設定する。</summary>
	/// <param name="rotate">設定する回転ベクトル。</param>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	/// <summary>平行移動（座標）を設定する。</summary>
	/// <param name="translate">設定する座標ベクトル。</param>
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	/// <summary>垂直方向（y軸）視野角を設定する。</summary>
	/// <param name="fov_y">ラジアン単位の視野角。</param>
	void SetFovY(const float& fov_y) { fov_y_ = fov_y; }
	/// <summary>アスペクト比を設定する（幅 / 高さ）。</summary>
	/// <param name="aspect_ratio">アスペクト比。</param>
	void SetAspectRatio(const float& aspect_ratio) { aspect_ratio_ = aspect_ratio; }
	/// <summary>ニアクリップ距離を設定する。</summary>
	/// <param name="near_clip">ニアクリップ距離（正の値）。</param>
	void SetNearClip(const float& near_clip) { near_clip_ = near_clip; }
	/// <summary>ファークリップ距離を設定する。</summary>
	/// <param name="far_clip">ファークリップ距離（ニアより大きい正の値）。</param>
	void SetFarClip(const float& far_clip) { far_clip_ = far_clip; }

	/// <summary>ワールド行列を取得する。</summary>
	/// <returns>現在の world 行列の const 参照。</returns>
	const Matrix4x4& GetWorldMatrix() const { return world_matrix_; }
	/// <summary>ビュー行列を取得する。</summary>
	/// <returns>現在の view 行列の const 参照。</returns>
	const Matrix4x4& GetViewMatrix() const { return view_matrix_; }
	/// <summary>プロジェクション行列を取得する。</summary>
	/// <returns>現在の projection 行列の const 参照。</returns>
	const Matrix4x4& GetProjectionMatrix() const { return projection_matrix_; }
	/// <summary>ビュー×プロジェクション行列を取得する。</summary>
	/// <returns>現在の viewProjection 行列の const 参照。</returns>
	const Matrix4x4& GetViewProjectionMatrix() const { return view_projection_matrix_; }
	/// <summary>回転を取得する。</summary>
	/// <returns>Transform に格納された回転ベクトルの const 参照。</returns>
	const Vector3& GetRotate() const { return transform_.rotate; }
	/// <summary>座標を取得する。</summary>
	/// <returns>Transform に格納された平行移動ベクトルの const 参照。</returns>
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

