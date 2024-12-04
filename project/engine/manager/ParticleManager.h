#pragma once
#include "DirectXBase.h"
#include "SrvManager.h"
#include "TextureManager.h"
#include "DirectXBase.h"
#include "Logger.h"
#include "Camera.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <unordered_map>
#include <string>
#include <random>

//頂点データ
struct VertexData {
	Vector4 position;
	Vector2 texcoord;
	Vector3 normal;
};
struct MaterialData
{
	std::string textureFilePath;
	uint32_t textureIndex = 0;
};

struct ModelData
{
	std::vector<VertexData>vertices;
	MaterialData material;
};
//パーティクル
struct Particle {
	Transform transform;
	Vector3 velocity;
	Vector4 color;
	float lifeTime;
	float currentTime;
};
struct ParticleForGPU {
	Matrix4x4 WVP;
	Matrix4x4 World;
	Vector4 color;
};
class ParticleManager
{
private:
	struct ParticleGroup {
		MaterialData materialData;//マテリアルデータ
		std::list<Particle>particles;//パーティクルリスト
		uint32_t SRVIndex;//インスタンシングデータ用SRVインデックス
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;//インスタンシングリソース
		uint32_t kNumInstance;//インスタンス数
		ParticleForGPU* instancingData;//インスタンシングデータを書き込むためのポインタ
	};
public:
	void Initialize(DirectXBase*directxBase,SrvManager*srvManager);

	void Update();

	void Draw();
	//パーティクルグループの生成
	void CreateparticleGroup(const std::string name, const std::string textureFilePath);
private:
	//ルートシグネチャの作成
	void GenerateRootSignature();
	//グラフィックスパイプラインの生成
	void GenerategraphicsPipeline();
	//頂点データ作成
	void VertexDataCreate();
private:
	DirectXBase* directxBase_;
	SrvManager* srvManager_;
	Camera* camera_;
	HRESULT hr;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    //バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	//乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	//グループコンテナ
	std::unordered_map<std::string, ParticleGroup>particleGroups;

	const uint32_t kNumMaxInstance = 100;
};


