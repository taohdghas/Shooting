#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>

// 頂点データ
/// <summary>
/// 頂点の位置、テクスチャ座標、法線情報を持つ構造体
/// </summary>
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};

// トランスフォーム
/// <summary>
/// スケール、回転、平行移動をまとめたトランスフォーム情報の構造体
/// </summary>
struct Transform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

// マテリアルデータ
/// <summary>
/// マテリアルの色、ライティング有効フラグ、UV変換行列、光沢度を持つ構造体
/// </summary>
struct Material {
	Vector4 color;
	int32_t enableLighting;
	float padding[3];
	Matrix4x4 uvTransform;
	float shininess;
};

// 座標変換行列データ
/// <summary>
/// WVP行列、ワールド行列、ワールド逆転置行列をまとめた構造体
/// </summary>
struct TransformationMatrix {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};

// マテリアルデータ
/// <summary>
/// マテリアルのテクスチャファイルパスとインデックスを持つ構造体
/// </summary>
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

// Node
/// <summary>
/// 階層構造を持つノード。ローカル行列、名前、子ノードを持つ構造体
/// </summary>
struct Node {
	Matrix4x4 localMatrix;
	std::string name;
	std::vector<Node>children;
};

// モデルデータ
/// <summary>
/// モデルの頂点配列、マテリアル、ルートノードを持つ構造体
/// </summary>
struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
	Node rootNode;
};

// ライト
/// <summary>
/// 平行光源の色、向き、輝度、有効フラグを持つ構造体
/// </summary>
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
/// <summary>
/// GPU用のカメラのワールド座標を持つ構造体
/// </summary>
struct CameraForGPU {
	Vector3 worldPosition;
};

// ポイントライト
/// <summary>
/// 点光源の色、位置、輝度、最大距離、減衰率、有効フラグを持つ構造体
/// </summary>
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
/// <summary>
/// スポットライトの色、位置、向き、輝度、距離、減衰率、角度、有効フラグを持つ構造体
/// </summary>
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
/// <summary>
/// 軸平行境界ボックス（AABB）の最小点と最大点を持つ構造体
/// </summary>
struct AABB {
	// 最小点
	Vector3 min;
	// 最大点
	Vector3 max;
};

// OBB
/// <summary>
/// 有向境界ボックス（OBB）の中心点、軸、サイズを持つ構造体
/// </summary>
struct OBB {
	// 中心点
	Vector3 center;
	// 座標軸
	Vector3 orientations[3];
	// 座標軸方向の長さの半分
	Vector3 size;
};

// 自キャラの生成データ
/// <summary>
/// プレイヤー生成時の平行移動、回転、スケーリング情報を持つ構造体
/// </summary>
struct PlayerSpawnData {
	// 平行移動
	Vector3 translation;
	// 回転角
	Vector3 rotation;
	// スケーリング
	Vector3 scaling;
};

// 敵用スプラインデータ
/// <summary>
/// 敵の移動用スプラインの制御点や名前、閉じているかどうかを持つ構造体
/// </summary>
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
/// <summary>
/// 敵キャラの名前、ファイル名、体力、位置、回転、スケーリング、所有レールを持つ構造体
/// </summary>
struct EnemySpawnData {
	std::string name;
	// ファイル名
	std::string fileName;
	// 体力
	int hp = 100;
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
/// <summary>
/// ボスキャラの名前、ファイル名、位置、回転、スケーリングを持つ構造体
/// </summary>
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
/// <summary>
/// レベル内のオブジェクト、プレイヤー、敵、ボスの生成情報をまとめた構造体
/// </summary>
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