#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "DirectionalLight.h"
#include "Transform.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"

class Object3dBase;
//3Dオブジェクト
class Object3d
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
	//座標変換行列データ
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
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
	void Initialize(Object3dBase*object3dBase);
	//更新
	void Update();
	//描画
	void Draw();
private:
	//頂点データ作成
	void VertexDataCreate();
	//マテリアル作成
	void MaterialCreate();
	//座標変換行列データ作成
	void TransformationCreate();
	//平行光源データ作成
	void DirectionalLightCreate();
	//.mtlファイルの読み取り
	static MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	//.objファイルの読み取り
	static ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename);
private:
	Object3dBase* object3dBase_ = nullptr;
	//objファイルのデータ
	ModelData modelData_;
	
	Transform transform_;
	Transform cameraTransform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>DirectionalLightResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	DirectionalLight* directionalLight = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

