#pragma once
#include "DirectXBase.h"
#include "Pso.h"
#include "Struct.h"

class Skybox
{
public:
	//初期化
	void Initialize(std::string textureFilePath);
	//更新
	void Update();
	//描画
	void Draw();
	//index作成
	void CreateIndex();
	//マテリアル作成
	void CreateMaterial();
	//座標変換行列データ作成
	void CreateTransformation();
	//頂点作成
	void CreateVertex();
public:
	Transform& GetTransform() { return transform_; }

private:
	DirectXBase* directxBase_ = nullptr;
	std::unique_ptr<Pso>pso_;
	VertexData* vertexData = nullptr;
	Material* materialData;
	Transform transform_;
	TransformationMatrix* transformationMatrixData;
	ModelData modelData;
	std::string textureFilePath_;
	std::vector<VertexData> vertices;
	uint32_t* indexData;
	std::vector<uint32_t>indices;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
};

