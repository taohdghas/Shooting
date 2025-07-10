#pragma once
#include "DirectXBase.h"
#include "Struct.h"
#include "Camera.h"

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
	//ルートシグネチャの作成
	void GenerateRootSignature();
	//グラフィックスパイプラインの生成
	void GenerateGraphicsPipeline();

private:
	VertexData* vertexData = nullptr;
	DirectXBase* directxBase_ = nullptr;
	Camera* camera_ = nullptr;
	Material* materialData;
	Transform transform_;
	TransformationMatrix* transformationMatrixData;
	ModelData modelData;
	std::string textureFilePath_;
	std::vector<VertexData> vertices;
	uint32_t* indexData;
	std::vector<uint32_t>indices;

	HRESULT hr;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
};

