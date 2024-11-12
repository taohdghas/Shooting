#pragma once
#include "Transform.h"
#include "Matrix4x4.h"
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
	void SetRotate(const Vector3& rotate) { transform.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { transform.translate = translate; }
	void SetFovY(const float& fovY) { FovY = fovY; }
	void SetAspectRatio(const float& aspectratio) { aspectRatio = aspectratio; }
	void SetNearClip(const float& nearclip){ nearClip = nearclip; }
	void SetFarClip(const float& farclip) { farClip = farclip; }
	///getter///
	const Matrix4x4& GetWorldMatrix()const { return worldMatrix; }
	const Matrix4x4& GetViewMatrix()const { return viewMatrix; }
	const Matrix4x4& GetProjectionMatrix()const { return projectionMatrix; }
	const Matrix4x4& GetViewProjectionMatrix()const { return viewProjectionMatrix; }
	const Vector3& GetRotate()const { return transform.rotate; }
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

