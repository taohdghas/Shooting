#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "DirectionalLight.h"
#include "Transform.h"
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"
#include "camera.h"

class Model;
class Object3dBase;
//3Dオブジェクト
class Object3d
{
public:
	//初期化
	void Initialize(Object3dBase*object3dBase);
	//更新
	void Update();
	//描画
	void Draw();
	///setter///
	void SetModel(const std::string& filePath);
	void SetScale(const Vector3& scale) { this->transform_.scale = scale; }
	void SetRotate(const Vector3& rotate) { this->transform_.rotate = rotate; }
	void SetTranslate(const Vector3& translate) { this->transform_.translate = translate; }
	void SetCamera(Camera* camera) { this->camera = camera; }
	///getter///
	const Vector3& GetScale()const { return transform_.scale; }
	const Vector3& GetRotate()const { return transform_.rotate; }
	const Vector3& GetTranslate()const { return transform_.translate; }
private:
	//座標変換行列データ作成
	void TransformationCreate();
	//平行光源データ作成
	void DirectionalLightCreate();
	//カメラデータ作成
	void CameraDataCreate();
	//ポイントライトデータ作成
	void PointLightCreate();
	//スポットライトデータ作成
	void SpotLightCreate();
private:
	Model* model_ = nullptr;
	Object3dBase* object3dBase_ = nullptr;
	Camera* camera = nullptr;
	//objファイルのデータ
	ModelData modelData_;
	
	Transform transform_;
	Transform cameraTransform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>DirectionalLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>cameraResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>PointLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>SpotLightResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	DirectionalLight* directionalLight = nullptr;
	CameraForGPU* cameraData = nullptr;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

