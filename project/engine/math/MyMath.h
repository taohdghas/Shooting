#pragma once
#include "Struct.h"
#include <corecrt_math.h>

/// <summary>
/// Vector3同士の加算
/// </summary>
/// <param name="v1">加算対象のベクトル1</param>
/// <param name="v2">加算対象のベクトル2</param>
/// <returns>加算結果のベクトル</returns>
inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

/// <summary>
/// Vector3に別のベクトルを加算して更新
/// </summary>
/// <param name="v1">加算対象かつ更新されるベクトル</param>
/// <param name="v2">加算するベクトル</param>
/// <returns>更新後のベクトル</returns>
inline Vector3& operator+=(Vector3& v1, const Vector3& v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    return v1;
}

/// <summary>
/// Vector3同士の減算
/// </summary>
/// <param name="v1">被減算ベクトル</param>
/// <param name="v2">減算するベクトル</param>
/// <returns>減算結果のベクトル</returns>
inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

/// <summary>
/// Vector3から別のベクトルを減算して更新
/// </summary>
/// <param name="v1">更新対象のベクトル</param>
/// <param name="v2">減算するベクトル</param>
/// <returns>更新後のベクトル</returns>
inline Vector3& operator-=(Vector3& v1, const Vector3& v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    return v1;
}

/// <summary>
/// Vector3とスカラー値の掛け算
/// </summary>
/// <param name="v">ベクトル</param>
/// <param name="s">スカラー値</param>
/// <returns>掛け算結果のベクトル</returns>
inline Vector3 operator*(const Vector3& v, float s) {
    return { v.x * s, v.y * s, v.z * s };
}

/// <summary>
/// Vector3にスカラー値を掛けて更新
/// </summary>
/// <param name="v">更新対象のベクトル</param>
/// <param name="s">スカラー値</param>
/// <returns>更新後のベクトル</returns>
inline Vector3& operator*=(Vector3& v, float s) {
    v.x *= s;
    v.y *= s;
    v.z *= s;
    return v;
}

/// <summary>
/// Vector3同士の掛け算（要素ごと）
/// </summary>
/// <param name="v1">ベクトル1</param>
/// <param name="v2">ベクトル2</param>
/// <returns>掛け算結果のベクトル</returns>
inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
    return { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
}

/// <summary>
/// Vector3同士の掛け算（要素ごと）を更新
/// </summary>
/// <param name="v1">更新対象のベクトル</param>
/// <param name="v2">掛けるベクトル</param>
/// <returns>更新後のベクトル</returns>
inline Vector3& operator*=(Vector3& v1, const Vector3& v2) {
    v1.x *= v2.x;
    v1.y *= v2.y;
    v1.z *= v2.z;
    return v1;
}

/// <summary>
/// 指定した値に対してイージング（EaseOutQuad）を適用した結果を返します。
/// - 0.0～1.0の範囲の値xに対して、加速→減速の曲線を返します。
/// </summary>
/// <param name="x">イージングを適用する0.0～1.0の値</param>
/// <returns>イージング後の値（0.0～1.0）</returns>
double easeOutQuad(double x);

/// <summary>
/// 指定した値に対してイージング（EaseInOutQuad）を適用した結果を返します。
/// - 0.0～1.0の範囲の値xに対して、加速→減速の曲線（前半は加速、後半は減速）を返します。
/// </summary>
/// <param name="x">イージングを適用する0.0～1.0の値</param>
/// <returns>イージング後の値（0.0～1.0）</returns>
double easeInOutQuad(double x);

namespace Math {
    /// <summary>ベクトルの長さを計算する</summary>
    /// <param name="v">計算対象のベクトル</param>
    /// <returns>ベクトルの長さ</returns>
    float Length(const Vector3& v);

    /// <summary>2つのベクトルの内積を計算する</summary>
    /// <param name="v1">ベクトル1</param>
    /// <param name="v2">ベクトル2</param>
    /// <returns>内積の結果</returns>
    float Dot(const Vector3& v1, const Vector3& v2);

    /// <summary>2つのベクトルの加算</summary>
    /// <param name="v1">ベクトル1</param>
    /// <param name="v2">ベクトル2</param>
    /// <returns>加算結果のベクトル</returns>
    Vector3 Add(const Vector3& v1, const Vector3& v2);

    /// <summary>2つのベクトルの減算</summary>
    /// <param name="v1">ベクトル1</param>
    /// <param name="v2">ベクトル2</param>
    /// <returns>減算結果のベクトル</returns>
    Vector3 Subtract(const Vector3& v1, const Vector3& v2);

    /// <summary>ベクトルの正規化</summary>
    /// <param name="v">正規化するベクトル</param>
    /// <returns>正規化されたベクトル</returns>
    Vector3 Normalize(const Vector3& v);

    /// <summary>2つのベクトルの要素ごとの掛け算</summary>
    /// <param name="v1">ベクトル1</param>
    /// <param name="v2">ベクトル2</param>
    /// <returns>掛け算結果のベクトル</returns>
    Vector3 Multiply(const Vector3& v1, const Vector3& v2);

    /// <summary>ベクトルとスカラー値の掛け算</summary>
    /// <param name="v">ベクトル</param>
    /// <param name="scalar">スカラー値</param>
    /// <returns>掛け算結果のベクトル</returns>
    Vector3 Multiply(const Vector3& v, const float scalar);

    /// <summary>ベクトルを行列で変換（座標変換）</summary>
    /// <param name="vector">変換対象のベクトル</param>
    /// <param name="matrix">変換行列</param>
    /// <returns>変換後のベクトル</returns>
    Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

    /// <summary>法線ベクトルを行列で変換</summary>
    /// <param name="v">法線ベクトル</param>
    /// <param name="m">変換行列</param>
    /// <returns>変換後の法線ベクトル</returns>
    Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

    /// <summary>2つのベクトルの外積を計算</summary>
    /// <param name="v1">ベクトル1</param>
    /// <param name="v2">ベクトル2</param>
    /// <returns>外積結果のベクトル</returns>
    Vector3 Cross(const Vector3& v1, const Vector3& v2);

    /// <summary>座標ベクトル(Vector4)を行列で変換</summary>
    /// <param name="v">変換対象のベクトル</param>
    /// <param name="m">変換行列</param>
    /// <returns>変換後のベクトル</returns>
    Vector4 Transform(const Vector4& v, const Matrix4x4& m);

    /// <summary>単位行列を作成</summary>
    /// <returns>4x4の単位行列</returns>
    Matrix4x4 MakeIdentity4x4();

    /// <summary>2つの行列の掛け算</summary>
    /// <param name="m1">行列1</param>
    /// <param name="m2">行列2</param>
    /// <returns>掛け算結果の行列</returns>
    Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

    /// <summary>平行移動行列を作成</summary>
    /// <param name="translate">平行移動量</param>
    /// <returns>平行移動行列</returns>
    Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

    /// <summary>拡大縮小行列を作成</summary>
    /// <param name="scale">拡大縮小量</param>
    /// <returns>拡大縮小行列</returns>
    Matrix4x4 MakeScaleMatrix(const Vector3& scale);

    /// <summary>x軸回転行列を作成</summary>
    /// <param name="radian">回転角度（ラジアン）</param>
    /// <returns>x軸回転行列</returns>
    Matrix4x4 MakeRotateXMatrix(float radian);

    /// <summary>y軸回転行列を作成</summary>
    /// <param name="radian">回転角度（ラジアン）</param>
    /// <returns>y軸回転行列</returns>
    Matrix4x4 MakeRotateYMatrix(float radian);

    /// <summary>z軸回転行列を作成</summary>
    /// <param name="radian">回転角度（ラジアン）</param>
    /// <returns>z軸回転行列</returns>
    Matrix4x4 MakeRotateZMatrix(float radian);

    /// <summary>ベクトルによる回転行列を作成</summary>
    /// <param name="rotate">回転角（x, y, z）</param>
    /// <returns>回転行列</returns>
    Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

    /// <summary>3Dアフィン変換行列を作成</summary>
    /// <param name="scale">拡大縮小ベクトル</param>
    /// <param name="rotate">回転ベクトル</param>
    /// <param name="translate">平行移動ベクトル</param>
    /// <returns>アフィン変換行列</returns>
    Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

    /// <summary>行列の逆行列を計算</summary>
    /// <param name="m">対象の行列</param>
    /// <returns>逆行列</returns>
    Matrix4x4 Inverse(const Matrix4x4& m);

    /// <summary>透視投影行列を作成</summary>
    /// <param name="fovY">垂直視野角（ラジアン）</param>
    /// <param name="aspectRatio">アスペクト比</param>
    /// <param name="nearClip">近クリップ距離</param>
    /// <param name="farClip">遠クリップ距離</param>
    /// <returns>透視投影行列</returns>
    Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

    /// <summary>正射影行列を作成</summary>
    /// <param name="left">左端</param>
    /// <param name="top">上端</param>
    /// <param name="right">右端</param>
    /// <param name="bottom">下端</param>
    /// <param name="nearClip">近クリップ距離</param>
    /// <param name="farClip">遠クリップ距離</param>
    /// <returns>正射影行列</returns>
    Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

    /// <summary>ビューポート行列を作成</summary>
    /// <param name="left">左端座標</param>
    /// <param name="top">上端座標</param>
    /// <param name="width">幅</param>
    /// <param name="height">高さ</param>
    /// <param name="minDepth">最小深度</param>
    /// <param name="maxDepth">最大深度</param>
    /// <returns>ビューポート行列</returns>
    Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

    /// <summary>行列の転置を計算</summary>
    /// <param name="m">対象の行列</param>
    /// <returns>転置行列</returns>
    Matrix4x4 Transpose(const Matrix4x4& m);

    /// <summary>OBB同士の衝突判定</summary>
    /// <param name="obb1">判定対象のOBB1</param>
    /// <param name="obb2">判定対象のOBB2</param>
    /// <returns>衝突していればtrue</returns>
    bool IsCollisionOBB(const OBB& obb1, const OBB& obb2);
}
