#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>
#include <map>

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
//Quaternion
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

//Keyframe
template <typename tValue>
struct Keyframe {
	float time;
	tValue value;
};
using KeyframeVector3 = Keyframe<Vector3>;
using KeyframeQuaternion = Keyframe<Quaternion>;

//Animation
template<typename tValue>
struct AnimationCurve {
	std::vector<Keyframe<tValue>>keyframes;
};

struct NodeAnimation {
	AnimationCurve<Vector3>translate;
	AnimationCurve<Quaternion>rotate;
	AnimationCurve<Vector3>scale;
};

struct Animation {
	float duration;//アニメーション全体の尺(単位は秒)
	std::map<std::string, NodeAnimation>nodeAnimations;
};