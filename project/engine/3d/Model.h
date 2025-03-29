#pragma once
#include "ModelBase.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//3Dモデル
class Model
{
private:

public:
	//初期化
	void Initialize(ModelBase* modelBase,const std::string&directorypath,const std::string&filename);
	//描画
	void Draw();
private:
	//頂点データ作成
	void VertexDataCreate();
	//マテリアル作成
	void MaterialCreate();
	//.mtlファイルの読み取り
	 MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み取り
	 ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	//assimpの構造体変換
	Node ReadNode(aiNode* node);
public:
	const ModelData& GetModelData()const { return modelData_; }
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

