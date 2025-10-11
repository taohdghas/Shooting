#pragma once
#include "Struct.h"
#include "WindowsAPI.h"

//カメラ
class Camera
{
public:
	//デフォルトコンストラクタ
	Camera();
	//更新
	void Update();

	///setter///

	//回転をセット
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	//座標をセット
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	//水平方向視野角セット
	void SetFovY(const float& fovY) { FovY = fovY; }
	//アスペクト比をセット
	void SetAspectRatio(const float& aspectratio) { aspectRatio = aspectratio; }
	//ニアクリップ距離セット
	void SetNearClip(const float& nearclip){ nearClip = nearclip; }
	//ファークリップ距離セット
	void SetFarClip(const float& farclip) { farClip = farclip; }

	///getter///

	//ワールド行列取得
	const Matrix4x4& GetWorldMatrix()const { return worldMatrix; }
	//ビュー行列取得
	const Matrix4x4& GetViewMatrix()const { return viewMatrix; }
	//プロジェクション行列取得
	const Matrix4x4& GetProjectionMatrix()const { return projectionMatrix; }
	//ビュープロジェクション行列取得
	const Matrix4x4& GetViewProjectionMatrix()const { return viewProjectionMatrix; }
	//回転取得
	const Vector3& GetRotate()const { return transform.rotate; }
	//座標を取得
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

