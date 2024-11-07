#pragma once
#include "ModelBase.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"


//3Dモデル
class Model
{
private:
	//頂点データ
	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	//マテリアルデータ
	struct Material
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
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
public:
	//初期化
	void Initialize(ModelBase* modelBase);
	//描画
	void Draw();
private:
	//頂点データ作成
	void VertexDataCreate();
	//マテリアル作成
	void MaterialCreate();
	//.mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み取り
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
private:
	//ModelBaseのポインタ
	ModelBase* modelBase_;
	//objファイルのデータ
	ModelData modelData_;
	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

