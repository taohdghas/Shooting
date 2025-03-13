#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>

//頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
//マテリアルデータ
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
};
//座標変換行列データ
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
};
//カメラ
struct CameraForGPU {
	Vector3 worldPosition;
};
//マテリアルデータ
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};
//モデルデータ
struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
};