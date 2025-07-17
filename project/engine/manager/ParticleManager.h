#pragma once
#include "DirectXBase.h"
#include "SrvManager.h"
#include "Pso.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Struct.h"

#include <unordered_map>
#include <string>
#include <random>

//パーティクル種類
enum class ParticleType {
	Normal,//通常
	Ring,//Ring型
	Cylinder,//Cylinder型
	Explosive,//爆発
};

class ParticleManager
{
public:

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
	struct AccelerationField {
		Vector3 acceleration;//加速度
		AABB area;//範囲
	};
	struct ParticleGroup {
		MaterialData materialData;//マテリアルデータ
		std::list<Particle>particles;//パーティクルリスト
		uint32_t SRVIndex;//インスタンシングデータ用SRVインデックス
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;//インスタンシングリソース
		uint32_t kNumInstance;//インスタンス数
		ParticleForGPU* instancingData ;//インスタンシングデータを書き込むためのポインタ
		ParticleType type;//パーティクルの種類
		ModelData modelData;
		Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource;//頂点リソース
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
public:
	//シングルトンインスタンスの取得
	static ParticleManager* GetInstance();
	//初期化
	void Initialize(DirectXBase* directxBase, SrvManager* srvManager, Camera* camera);
	//終了
	void Finalize();
	//シーン終了時に呼ぶ
	void Clear();
	//更新
	void Update();
	//描画
	void Draw();
	//パーティクルグループの生成
	void CreateparticleGroup(const std::string name, const std::string textureFilePath,ParticleType type);
	//パーティクル生成関数
	Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate, ParticleType type);
	//パーティクルの発生
	void Emit(const std::string name, const Vector3& position, uint32_t count);
	//ParticleがFieldの範囲内か判定
	bool IsCollision(const AABB& aabb, const Vector3& point);
public:
	//ParticleTypeのゲッター
	ParticleType GetParticleType(const std::string& name);
private:
	//頂点データ作成
	void VertexDataCreate(ModelData&modelData);
	//Ringの頂点データ作成
	void RingVertexDataCreate(ModelData&modelData);
	//Cylinderの頂点データ作成
	void CylinderVertexDataCreate(ModelData&modelData);
	//マテリアルデータ作成
	void MaterialCreate();
private:

	static ParticleManager* instance;
	DirectXBase* directxBase_;
	SrvManager* srvManager_;
	std::unique_ptr<Pso>pso_;
	Camera* camera_;
	ModelData modelData;
	AABB aabb_;
	AccelerationField accelerationfield_;
	HRESULT hr;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	//乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	//グループコンテナ
	std::unordered_map<std::string, ParticleGroup>particleGroups;

	const uint32_t kNumMaxInstance =128;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//billboardMatrix切り替え
	bool useBillboard = true;
};


