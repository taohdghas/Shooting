#pragma once
#include "MyMath.h"
#include <cassert>

namespace Math {
	// EaseOutQuadイージング関数
    // x（0.0～1.0）に対して加速→減速のカーブを返す
	double Math::easeOutQuad(double x) {
		// (1-x)^2で減速カーブを作り、1.0から引くことで加速→減速のイージングを実現
		return 1.0 - (1.0 - x) * (1.0 - x);
	}

	// EaseInOutQuadイージング関数
	// x（0.0～1.0）に対して前半は加速、後半は減速のカーブを返す
	double Math::easeInOutQuad(double x) {
		// xが0.5未満なら加速（前半）、0.5以上なら減速（後半）
		if (x < 0.5)
			return 2 * x * x;
		else
			return 1 - pow(-2 * x + 2, 2) / 2;
	}
	//長さ
	float Math::Length(const Vector3& v) {
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	//内積
	float Math::Dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	//乗算(Vector同士)
	Vector2 Math::Multiply(const Vector2& v1, const Vector2& v2) {
		return { v1.x * v2.x, v1.y * v2.y };
	}
	//乗算(Vector*scaler)
	Vector2 Math::MultiplyScalar(const Vector2& v, const float scalar) {
		return { v.x * scalar, v.y * scalar };
	}
	//加算
	Vector3 Math::Add(const Vector3& v1, const Vector3& v2) {
		return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	}
	//減算
	Vector3 Math::Subtract(const Vector3& v1, const Vector3& v2) {
		return { v1.x - v2.x,v1.y - v2.y,v1.z - v2.z };
	}
	//正規化
	Vector3 Math::Normalize(const Vector3& v) {
		Vector3 result;
		float length = Length(v);
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
		return result;
	}
	//乗算(Vector同士)
	Vector3 Math::Multiply(const Vector3& v1, const Vector3& v2) {
		Vector3 result{};
		result.x = v1.x * v2.x;
		result.y = v1.y * v2.y;
		result.z = v1.z * v2.z;
		return result;
	}
	//乗算(Vector*scaler)
	Vector3 Math::MultiplyScalar(const Vector3& v, const float scalar) {
		Vector3 result{};
		result.x = v.x * scalar;
		result.y = v.y * scalar;
		result.z = v.z * scalar;
		return result;
	}
	//座標ベクトル変換
	Vector3 Math::Transform(const Vector3& vector, const Matrix4x4& matrix) {
		Vector3 result;
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;
		return result;
	}
	//法線ベクトル変換
	Vector3 Math::TransformNormal(const Vector3& v, const Matrix4x4& m) {
		Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] };
		return result;
	}
	//外積
	Vector3 Math::Cross(const Vector3& v1, const Vector3& v2) {
		Vector3 result;
		result.x = v1.y * v2.z - v1.z * v2.y;
		result.y = v1.z * v2.x - v1.x * v2.z;
		result.z = v1.x * v2.y - v1.y * v2.x;
		return result;
	}

	Vector3 Math::Lerp(const Vector3& a, const Vector3& b, float t) {
		return a * (1.0f - t) + b * t;
	}

	//CatMullRom補間
	Vector3 Math::CatmullRom(
		const Vector3& p0,
		const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;

		return
			(p1 * 2.0f +
				(p2 - p0) * t +
				(p0 * 2.0f - p1 * 5.0f + p2 * 4.0f - p3) * t2 +
				(-p0 + p1 * 3.0f - p2 * 3.0f + p3) * t3) * 0.5f;
	}

	//座標ベクトル変換
	Vector4 Math::Transform(const Vector4& v, const Matrix4x4& m) {
		return {
			v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
			v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
			v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3],
		};
	}
	//単位行列
	Matrix4x4 Math::MakeIdentity4x4() {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
		return result;
	}
	//積
	Matrix4x4 Math::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result{};
		for (int row = 0; row < 4; ++row) {
			for (int column = 0; column < 4; ++column) {
				for (int i = 0; i < 4; ++i) {
					result.m[row][column] += m1.m[row][i] * m2.m[i][column];
				}
			}
		}
		return result;
	}

	//平行移動行列
	Matrix4x4 Math::MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 translateMatrix;

		translateMatrix.m[0][0] = 1;
		translateMatrix.m[0][1] = 0;
		translateMatrix.m[0][2] = 0;
		translateMatrix.m[0][3] = 0;
		translateMatrix.m[1][0] = 0;
		translateMatrix.m[1][1] = 1;
		translateMatrix.m[1][2] = 0;
		translateMatrix.m[1][3] = 0;
		translateMatrix.m[2][0] = 0;
		translateMatrix.m[2][1] = 0;
		translateMatrix.m[2][2] = 1;
		translateMatrix.m[2][3] = 0;
		translateMatrix.m[3][0] = translate.x;
		translateMatrix.m[3][1] = translate.y;
		translateMatrix.m[3][2] = translate.z;
		translateMatrix.m[3][3] = 1;
		return translateMatrix;
	}
	//拡大縮小行列
	Matrix4x4 Math::MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 scaleMatrix;
		scaleMatrix.m[0][0] = scale.x;
		scaleMatrix.m[0][1] = 0.0f;
		scaleMatrix.m[0][2] = 0.0f;
		scaleMatrix.m[0][3] = 0.0f;
		scaleMatrix.m[1][0] = 0.0f;
		scaleMatrix.m[1][1] = scale.y;
		scaleMatrix.m[1][2] = 0.0f;
		scaleMatrix.m[1][3] = 0.0f;
		scaleMatrix.m[2][0] = 0.0f;
		scaleMatrix.m[2][1] = 0.0f;
		scaleMatrix.m[2][2] = scale.z;
		scaleMatrix.m[2][3] = 0.0f;
		scaleMatrix.m[3][0] = 0.0f;
		scaleMatrix.m[3][1] = 0.0f;
		scaleMatrix.m[3][2] = 0.0f;
		scaleMatrix.m[3][3] = 1.0f;
		return scaleMatrix;
	}
	//x軸回転行列
	Matrix4x4 Math::MakeRotateXMatrix(float radian) {
		Matrix4x4 result;
		float c = float(cos(radian));
		float s = float(sin(radian));

		result.m[0][0] = 1.0f;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = c;
		result.m[1][2] = s;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = -s;
		result.m[2][2] = c;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;

		return result;
	}
	//y軸回転行列
	Matrix4x4 Math::MakeRotateYMatrix(float radian) {
		Matrix4x4 result;
		float c = float(cos(radian));
		float s = float(sin(radian));

		result.m[0][0] = c;
		result.m[0][1] = 0.0f;
		result.m[0][2] = -s;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = 1.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = s;
		result.m[2][1] = 0.0f;
		result.m[2][2] = c;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;

		return result;
	}
	//z軸回転行列
	Matrix4x4 Math::MakeRotateZMatrix(float radian) {
		Matrix4x4 result;
		float c = float(cos(radian));
		float s = float(sin(radian));

		result.m[0][0] = c;
		result.m[0][1] = s;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = -s;
		result.m[1][1] = c;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = 1.0f;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;

		return result;
	}

	//回転行列
	Matrix4x4 Math::MakeRotateMatrix(const Vector3& rotate) {
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 result = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
		return result;
	}

	//3次元アフィン変換行列
	Matrix4x4 Math::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
		Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix)); // 回転の順序を修正
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		Matrix4x4 affineMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

		return affineMatrix;
	}
	//逆行列
	Matrix4x4 Math::Inverse(const Matrix4x4& m) {
		float determinant =
			+m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]
			+ m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]
			+ m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]

			- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]
			- m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]
			- m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]

			- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]
			- m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]
			- m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]

			+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]
			+ m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]
			+ m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]

			+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]
			+ m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]
			+ m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]

			- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]
			- m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]
			- m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]

			- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]
			- m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]
			- m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]

			+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]
			+ m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]
			+ m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

		Matrix4x4 result = {};
		float recpDeterminant = 1.0f / determinant;
		result.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
			m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
			m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
		result.m[0][1] = (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
			m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
			m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
		result.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
			m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
			m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) * recpDeterminant;
		result.m[0][3] = (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
			m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
			m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]) * recpDeterminant;

		result.m[1][0] = (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
			m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
			m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
		result.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
			m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
			m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) * recpDeterminant;
		result.m[1][2] = (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
			m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
			m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]) * recpDeterminant;
		result.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
			m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
			m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) * recpDeterminant;

		result.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
			m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
			m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) * recpDeterminant;
		result.m[2][1] = (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
			m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
			m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]) * recpDeterminant;
		result.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
			m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
			m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) * recpDeterminant;
		result.m[2][3] = (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
			m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
			m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]) * recpDeterminant;

		result.m[3][0] = (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
			m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
			m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]) * recpDeterminant;
		result.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
			m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
			m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) * recpDeterminant;
		result.m[3][2] = (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
			m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
			m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]) * recpDeterminant;
		result.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
			m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
			m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) * recpDeterminant;

		return result;
	}
	//透視投影行列
	Matrix4x4 Math::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		Matrix4x4 perspectiveMatrix;
		perspectiveMatrix.m[0][0] = 1.0f / float((aspectRatio * tan(fovY / 2.0f)));
		perspectiveMatrix.m[0][1] = 0;
		perspectiveMatrix.m[0][2] = 0;
		perspectiveMatrix.m[0][3] = 0;
		perspectiveMatrix.m[1][0] = 0;
		perspectiveMatrix.m[1][1] = 1.0f / float(tan(fovY / 2.0f));
		perspectiveMatrix.m[1][2] = 0;
		perspectiveMatrix.m[1][3] = 0;
		perspectiveMatrix.m[2][0] = 0;
		perspectiveMatrix.m[2][1] = 0;
		perspectiveMatrix.m[2][2] = farClip / (farClip - nearClip);
		perspectiveMatrix.m[2][3] = 1;
		perspectiveMatrix.m[3][0] = 0;
		perspectiveMatrix.m[3][1] = 0;
		perspectiveMatrix.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
		perspectiveMatrix.m[3][3] = 0;
		return perspectiveMatrix;
	}
	//正射影行列
	Matrix4x4 Math::MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
		Matrix4x4 orthoMatrix;
		orthoMatrix.m[0][0] = 2.0f / (right - left);
		orthoMatrix.m[0][1] = 0;
		orthoMatrix.m[0][2] = 0;
		orthoMatrix.m[0][3] = 0;
		orthoMatrix.m[1][0] = 0;
		orthoMatrix.m[1][1] = 2.0f / (top - bottom);
		orthoMatrix.m[1][2] = 0;
		orthoMatrix.m[1][3] = 0;
		orthoMatrix.m[2][0] = 0;
		orthoMatrix.m[2][1] = 0;
		orthoMatrix.m[2][2] = 1.0f / (farClip - nearClip);
		orthoMatrix.m[2][3] = 0;
		orthoMatrix.m[3][0] = (left + right) / (left - right);
		orthoMatrix.m[3][1] = (top + bottom) / (bottom - top);
		orthoMatrix.m[3][2] = nearClip / (nearClip - farClip);
		orthoMatrix.m[3][3] = 1;
		return orthoMatrix;
	}
	//ビューポート行列
	Matrix4x4 Math::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
		Matrix4x4 viewportMatrix;
		viewportMatrix.m[0][0] = width / 2.0f;
		viewportMatrix.m[0][1] = 0;
		viewportMatrix.m[0][2] = 0;
		viewportMatrix.m[0][3] = 0;
		viewportMatrix.m[1][0] = 0;
		viewportMatrix.m[1][1] = -height / 2.0f;
		viewportMatrix.m[1][2] = 0;
		viewportMatrix.m[1][3] = 0;
		viewportMatrix.m[2][0] = 0;
		viewportMatrix.m[2][1] = 0;
		viewportMatrix.m[2][2] = maxDepth - minDepth;
		viewportMatrix.m[2][3] = 0;
		viewportMatrix.m[3][0] = left + width / 2.0f;
		viewportMatrix.m[3][1] = top + height / 2.0f;
		viewportMatrix.m[3][2] = minDepth;
		viewportMatrix.m[3][3] = 1;
		return viewportMatrix;
	}
	//転置行列
	Matrix4x4 Math::Transpose(const Matrix4x4& m) {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m.m[j][i];
			}
		}
		return result;
	}
	//OBB同士の衝突判定
	bool IsCollisionOBB(const OBB& obb1, const OBB& obb2) {
		const float EPSILON = 1e-6f;

		const Vector3& C1 = obb1.center;
		const Vector3& C2 = obb2.center;
		const Vector3* A = obb1.orientations;
		const Vector3* B = obb2.orientations;
		const Vector3& aExt = obb1.size;
		const Vector3& bExt = obb2.size;

		//中心差ベクトル
		Vector3 t = Subtract(C2, C1);
		//obb1のローカル空間に変換
		t = { Dot(t, A[0]), Dot(t, A[1]), Dot(t, A[2]) };

		//回転行列 R[i][j] = Ai・Bj
		float R[3][3], AbsR[3][3];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				R[i][j] = Dot(A[i], B[j]);
				AbsR[i][j] = abs(R[i][j]) + EPSILON;
			}
		}

		float ra, rb;
		auto GetExt = [](const Vector3& v, int idx) -> float {
			return (idx == 0) ? v.x : (idx == 1 ? v.y : v.z);
			};

		//obb1の軸
		for (int i = 0; i < 3; i++) {
			ra = GetExt(aExt, i);
			rb = bExt.x * AbsR[i][0] + bExt.y * AbsR[i][1] + bExt.z * AbsR[i][2];
			if (fabs(GetExt(t, i)) > ra + rb) return false;
		}
		//obb2の軸
		for (int j = 0; j < 3; j++) {
			ra = aExt.x * AbsR[0][j] + aExt.y * AbsR[1][j] + aExt.z * AbsR[2][j];
			rb = GetExt(bExt, j);
			if (fabs(t.x * R[0][j] + t.y * R[1][j] + t.z * R[2][j]) > ra + rb) return false;
		}
		//外積軸
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				ra = GetExt(aExt, (i + 1) % 3) * AbsR[(i + 2) % 3][j] +
					GetExt(aExt, (i + 2) % 3) * AbsR[(i + 1) % 3][j];
				rb = GetExt(bExt, (j + 1) % 3) * AbsR[i][(j + 2) % 3] +
					GetExt(bExt, (j + 2) % 3) * AbsR[i][(j + 1) % 3];
				float tval = (float)fabs(
					GetExt(t, (i + 2) % 3) * R[(i + 1) % 3][j] -
					GetExt(t, (i + 1) % 3) * R[(i + 2) % 3][j]
				);
				if (tval > ra + rb) return false;
			}
		}

		return true;
	}
}


