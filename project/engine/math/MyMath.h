#pragma once
#include "Struct.h"
#include <corecrt_math.h>

/// <summary>
/// Vector3同士の加算
/// </summary>
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// <summary>
/// Vector3に別のベクトルを加算して更新
/// </summary>
inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

/// <summary>
/// Vector3同士の減算
/// </summary>
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}
/// <summary>
/// Vector3 の単項マイナス（符号反転）
/// </summary>
inline Vector3 operator-(const Vector3& v) {
    return { -v.x, -v.y, -v.z };
}

/// <summary>
/// Vector3から別のベクトルを減算して更新
/// </summary>
inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

/// <summary>
/// Vector3とスカラー値の掛け算
/// </summary>
inline Vector3 operator*(const Vector3& v, float s) {
    return { v.x * s, v.y * s, v.z * s };
}

/// <summary>
/// Vector3にスカラー値を掛けて更新
/// </summary>
inline Vector3& operator*=(Vector3& v, float s) {
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

/// <summary>
/// Vector3同士の掛け算（要素ごと）
/// </summary>
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
    return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

/// <summary>
/// Vector3同士の掛け算（要素ごと）を更新
/// </summary>
inline Vector3& operator*=(Vector3& v1, const Vector3& v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    return v1;
}

/// <summary>
/// 指定した値に対してイージング（EaseOutQuad）を適用した結果を返します。
/// </summary>
double easeOutQuad(double x);

/// <summary>
/// 指定した値に対してイージング（EaseInOutQuad）を適用した結果を返します
/// </summary>
double easeInOutQuad(double x);

namespace Math {
    /// <summary>ベクトルの長さを計算する</summary>
    float Length(const Vector3& v);

    /// <summary>2つのベクトルの内積を計算する</summary>
    float Dot(const Vector3& v1, const Vector3& v2);

	/// <summary>2つのベクトルの加算</summary>
    Vector2 Multiply(const Vector2& v1, const Vector2& v2);

	/// <summary>ベクトルとスカラー値の掛け算</summary>
	Vector2 MultiplyScalar(const Vector2& v, const float scalar);

    /// <summary>2つのベクトルの加算</summary>
    Vector3 Add(const Vector3& v1, const Vector3& v2);

    /// <summary>2つのベクトルの減算</summary>
    Vector3 Subtract(const Vector3& v1, const Vector3& v2);

    /// <summary>ベクトルの正規化</summary>
    Vector3 Normalize(const Vector3& v);

    /// <summary>2つのベクトルの要素ごとの掛け算</summary>
    Vector3 Multiply(const Vector3& v1, const Vector3& v2);

    /// <summary>ベクトルとスカラー値の掛け算</summary>
    Vector3 MultiplyScalar(const Vector3& v, const float scalar);

    /// <summary>ベクトルを行列で変換（座標変換）</summary>
    Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

    /// <summary>法線ベクトルを行列で変換</summary>
    Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

    /// <summary>2つのベクトルの外積を計算</summary>
    Vector3 Cross(const Vector3& v1, const Vector3& v2);

	/// <summary>Catmull-Rom補間</summary>
    Vector3 CatmullRom(const Vector3& p0, const Vector3& p1,const Vector3& p2,const Vector3& p3,float t);

    /// <summary>座標ベクトル(Vector4)を行列で変換</summary>
    Vector4 Transform(const Vector4& v, const Matrix4x4& m);

    /// <summary>単位行列を作成</summary>
    Matrix4x4 MakeIdentity4x4();

    /// <summary>2つの行列の掛け算</summary>
    Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

    /// <summary>平行移動行列を作成</summary>
    Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

    /// <summary>拡大縮小行列を作成</summary>
    Matrix4x4 MakeScaleMatrix(const Vector3& scale);

    /// <summary>x軸回転行列を作成</summary>
    Matrix4x4 MakeRotateXMatrix(float radian);

    /// <summary>y軸回転行列を作成</summary>
    Matrix4x4 MakeRotateYMatrix(float radian);

    /// <summary>z軸回転行列を作成</summary>
    Matrix4x4 MakeRotateZMatrix(float radian);

    /// <summary>ベクトルによる回転行列を作成</summary>
    Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

    /// <summary>3Dアフィン変換行列を作成</summary>
    Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

    /// <summary>行列の逆行列を計算</summary>
    Matrix4x4 Inverse(const Matrix4x4& m);

    /// <summary>透視投影行列を作成</summary>
    Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

    /// <summary>正射影行列を作成</summary>
    Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

    /// <summary>ビューポート行列を作成</summary>
    Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

    /// <summary>行列の転置を計算</summary>
    Matrix4x4 Transpose(const Matrix4x4& m);

    /// <summary>OBB同士の衝突判定</summary>
    bool IsCollisionOBB(const OBB& obb1, const OBB& obb2);
}
