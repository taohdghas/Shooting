#pragma once
#include "Struct.h"
#include "WindowsAPI.h"

//カメラ
class Camera
{
public:
	/// <summary>
	/// デフォルトコンストラクタ。
	/// - 既定のスケール/回転/位置で Transform を初期化する。
	/// - 視野角は 0.45、アスペクト比は __WindowsAPI::kClientWidth / kClientHeight__、ニア/ファークリップは 0.1 / 100.0 を設定する。
	/// - world/view/projection/viewProjection 行列を初期値で生成する（Math ユーティリティを使用）。
	/// </summary>
	Camera();
	/// <summary>
	/// 毎フレーム更新する。
	/// - worldMatrix を Transform（スケール/回転/平行移動）から再生成する。
	/// - viewMatrix を worldMatrix の逆行列として計算する。
	/// - projectionMatrix を FovY / aspectRatio / nearClip / farClip から計算する。
	/// - viewProjectionMatrix を viewMatrix と projectionMatrix の積として更新する。
	/// </summary>
	void Update();

	///setter///

	/// <summary>
	/// 回転を設定する（ラジアン単位などはプロジェクトの規約に従う）。
	/// </summary>
	/// <param name="rotate">設定する回転ベクトル。</param>
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	/// <summary>
	/// 平行移動（座標）を設定する。
	/// </summary>
	/// <param name="translate">設定する座標ベクトル。</param>
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	/// <summary>
	/// 垂直方向（y軸）視野角を設定する。
	/// </summary>
	/// <param name="fovY">ラジアン単位の視野角。</param>
	void SetFovY(const float& fovY) { FovY = fovY; }
	/// <summary>
	/// アスペクト比を設定する（幅 / 高さ）。
	/// </summary>
	/// <param name="aspectratio">アスペクト比。</param>
	void SetAspectRatio(const float& aspectratio) { aspectRatio = aspectratio; }
	/// <summary>
	/// ニアクリップ距離を設定する。
	/// </summary>
	/// <param name="nearclip">ニアクリップ距離（正の値）。</param>
	void SetNearClip(const float& nearclip){ nearClip = nearclip; }
	/// <summary>
	/// ファークリップ距離を設定する。
	/// </summary>
	/// <param name="farclip">ファークリップ距離（ニアより大きい正の値）。</param>
	void SetFarClip(const float& farclip) { farClip = farclip; }

	///getter///

	/// <summary>
	/// ワールド行列を取得する。
	/// </summary>
	/// <returns>現在の world 行列の const 参照。</returns>
	const Matrix4x4& GetWorldMatrix()const { return worldMatrix; }
	/// <summary>
	/// ビュー行列を取得する。
	/// </summary>
	/// <returns>現在の view 行列の const 参照。</returns>
	const Matrix4x4& GetViewMatrix()const { return viewMatrix; }
	/// <summary>
	/// プロジェクション行列を取得する。
	/// </summary>
	/// <returns>現在の projection 行列の const 参照。</returns>
	const Matrix4x4& GetProjectionMatrix()const { return projectionMatrix; }
	/// <summary>
	/// ビュー×プロジェクション行列を取得する。
	/// </summary>
	/// <returns>現在の viewProjection 行列の const 参照。</returns>
	const Matrix4x4& GetViewProjectionMatrix()const { return viewProjectionMatrix; }
	/// <summary>
	/// 回転を取得する。
	/// </summary>
	/// <returns>Transform に格納された回転ベクトルの const 参照。</returns>
	const Vector3& GetRotate()const { return transform.rotate; }
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>Transform に格納された平行移動ベクトルの const 参照。</returns>
	const Vector3& GetTranslate()const { return transform.translate; }
private:
	Transform transform;
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;
	//水平方向視野角
	float FovY;
	//アスペクト比
	float aspectRatio;
	//ニアクリップ距離
	float nearClip;
	//ファークリップ距離
	float farClip;
};

