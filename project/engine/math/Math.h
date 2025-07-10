#pragma once
#include "Struct.h"
#include <corecrt_math.h>

// Vector3の足し算
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
	v1.x += v2.x;
	v1.y += v2.x;
	v1.z += v2.x;
	return v1;
}

// Vector3の引き算
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

inline Vector3 operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;
	return v1;
}

// Vector3の掛け算(スカラー)
inline Vector3 operator*(const Vector3& v, float s) {
	Vector3 result{};
	result.x = v.x * s;
	result.y = v.y * s;
	result.z = v.z * s;
	return result;
}

inline Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

// Vector3同士の掛け算
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}
inline Vector3& operator*=(Vector3& v1, const Vector3& v2) {
	v1.x *= v2.x;
	v1.y *= v2.y;
	v1.z *= v2.z;
	return v1;
}


namespace Math {

	float Length(const Vector3& v);
	float Dot(const Vector3& v1, const Vector3& v2);
	//加算
	Vector3 Add(const Vector3& v1, const Vector3& v2);
	//減算
	Vector3 Subtract(const Vector3& v1, const Vector3& v2);
	//正規化
	Vector3 Normalize(const Vector3& v);
	Vector3 Multiply(const Vector3& v1, const Vector3& v2);
	Vector3 Multiply(const Vector3& v, const float scalar);
	//座標ベクトル変換
	Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	//法線ベクトル変換
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);
	Vector3 Cross(const Vector3& v1, const Vector3& v2);
	//単位行列
	Matrix4x4 MakeIdentity4x4();

	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

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
	Matrix4x4 MakeRotateMatrix(const Vector3 & rotate);
	//3次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	//逆行列
	Matrix4x4 Inverse(const Matrix4x4& m);

	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	//ビューポート行列
	Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);
	//転置行列
	Matrix4x4 Transpose(const Matrix4x4& m);
	//OBB同士の当たり判定
	bool IsCollisionOBB(const OBB& obb1, const OBB& obb2);
}