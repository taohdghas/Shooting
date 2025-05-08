#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <corecrt_math.h>

namespace Math {

	Vector3 Add(const Vector3& v1, const Vector3& v2);
	//単位行列
	Matrix4x4 MakeIdentity4x4();

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	float Length(const Vector3& v);

	Vector3 Normalize(const Vector3& v);

	//平行移動行列
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
	//拡大縮小行列
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	//x軸回転行列
	Matrix4x4 MakeRotateXMatrix(float radian);
	//y軸回転行列
	Matrix4x4 MakeRotateYMatrix(float radian);
	//z軸回転行列
	Matrix4x4 MakeRotateZMatrix(float radian);
    //回転行列
	Matrix4x4 MakeRotateMatrix(const Vector3& rotate);
	//3次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	//逆行列
	Matrix4x4 Inverse(const Matrix4x4& m);

	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	//転置行列
	Matrix4x4 Transpose(const Matrix4x4& m);
}