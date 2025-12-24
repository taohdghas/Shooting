#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>

// 頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

// トランスフォーム
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// マテリアルデータ
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
};

// 座標変換行列データ
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};

// マテリアルデータ
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

// Node
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};

// モデルデータ
struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
	Node rootNode;
};

// ライト
struct DirectionalLight {
	// ライトの色
	Vector4 color;
	// ライトの向き
	Vector3 direction;
	// 輝度
	float intensity;
	// 有効か
	int enable;
};

// カメラ
struct CameraForGPU {
	Vector3 worldPosition;
};

// ポイントライト
struct PointLight {
	// ライトの色
	Vector4 color;
	// ライトの位置
	Vector3 position;
	// 輝度
	float intensity;
	// ライトの届く最大距離
	float radius;
	// 減衰率
	float decay;
	float padding[2];
	// 有効か
	int enable;
};

// スポットライト
struct SpotLight {
	// ライトの色
	Vector4 color;
	// ライトの位置
	Vector3 position;
	// 輝度
	float intensity;
	// スポットライト
	Vector3 direction;
	// ライトの届く最大距離
	float distance;
	// 減衰率
	float decay;
	// スポットライトの余弦
	float cosAngle;
	float cosFalloffStart;
	float padding[2];
	// 有効か
	int enable;
};

// AABB
struct AABB {
	// 最小点
	Vector3 min;
	// 最大点
	Vector3 max;
};

// OBB
struct OBB {
	// 中心点
	Vector3 center;
	// 座標軸
	Vector3 orientations[3];
	// 座標軸方向の長さの半分
	Vector3 size;
};

// 自キャラの生成データ
struct PlayerSpawnData {
	// 平行移動
	Vector3 translation;
	// 回転角
	Vector3 rotation;
	// スケーリング
	Vector3 scaling;
};

// 敵用スプラインデータ
struct EnemyRailData {
	// RailCurveObj の名前
	std::string name;
	// ワールド座標の制御点
	std::vector<Vector3> controlPoints;
	// 制御点に対応するオブジェクト名
	std::vector<std::string> railPointNames;
	// スプラインが閉じているか
	bool closed;
};

// 敵キャラの生成データ
struct EnemySpawnData {
	std::string name;
	// ファイル名
	std::string fileName;
	// 平行移動
	Vector3 translation;
	// 回転角
	Vector3 rotation;
	// スケーリング			
	Vector3 scaling;
	// 所有するレール
	std::vector<EnemyRailData> rails;
};

// ボスキャラの生成データ
struct BossSpawnData {
	std::string name;
	// ファイル名
	std::string fileName;
	// 平行移動
	Vector3 translation;
	// 回転角
	Vector3 rotation;
	// スケーリング
	Vector3 scaling;
};


// レベルデータ
struct LevelData {
	// オブジェクト1個分のデータ
	struct ObjectData {
		// ファイル名
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};
	// オブジェクトのコンテナ
	std::vector<ObjectData>objects;
	// 自キャラ配列
	std::vector<PlayerSpawnData>players;
	// 敵配列
	std::vector<EnemySpawnData>enemies;
	// ボス配列
	std::vector<BossSpawnData> bosses;
};