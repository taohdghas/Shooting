#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <corecrt_math.h>

namespace Math {
	//単位行列
	Matrix4x4 MakeIdentity4x4() {
		Matrix4x4 result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = (i == j) ? 1.0f : 0.0f;
			}
		}
		return result;
	}
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
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
	float Length(const Vector3& v) {

		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	Vector3 Normalize(const Vector3& v) {
		Vector3 result;
		float length = Length(v);
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
		return result;
	}
	//平行移動行列
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
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
	Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
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
	Matrix4x4 MakeRotateXMatrix(float radian) {
		Matrix4x4 result;
		float c = cos(radian);
		float s = sin(radian);

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
	Matrix4x4 MakeRotateYMatrix(float radian) {
		Matrix4x4 result;
		float c = cos(radian);
		float s = sin(radian);

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
	Matrix4x4 MakeRotateZMatrix(float radian) {
		Matrix4x4 result;
		float c = cos(radian);
		float s = sin(radian);

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
	//3次元アフィン変換行列
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
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
	Matrix4x4 Inverse(const Matrix4x4& m) {
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
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		Matrix4x4 perspectiveMatrix;
		perspectiveMatrix.m[0][0] = 1.0f / (aspectRatio * tan(fovY / 2.0f));
		perspectiveMatrix.m[0][1] = 0;
		perspectiveMatrix.m[0][2] = 0;
		perspectiveMatrix.m[0][3] = 0;
		perspectiveMatrix.m[1][0] = 0;
		perspectiveMatrix.m[1][1] = 1.0f / tan(fovY / 2.0f);
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
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
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
}