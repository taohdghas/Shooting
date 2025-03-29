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
	float shininess;
};
//座標変換行列データ
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};
//マテリアルデータ
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};
//Node
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};
//モデルデータ
struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
	Node rootNode;
};
//ライト
struct DirectionalLight {
	Vector4 color;//ライトの色
	Vector3 direction;//ライトの向き
	float intensity;//輝度
};
//カメラ
struct CameraForGPU {
	Vector3 worldPosition;
};
//ポイントライト
struct PointLight {
	Vector4 color;//ライトの色
	Vector3 position;//ライトの位置
	float intensity;//輝度
	float radius;//ライトの届く最大距離
	float decay;//減衰率
	float padding[2];
};
//スポットライト
struct SpotLight {
	Vector4 color;//ライトの色
	Vector3 position;//ライトの位置
	float intensity;//輝度
	Vector3 direction;//スポットライト
	float distance;//ライトの届く最大距離
	float decay;//減衰率
	float cosAngle;//スポットライトの余弦
	float cosFalloffStart;
	float padding[2];
};