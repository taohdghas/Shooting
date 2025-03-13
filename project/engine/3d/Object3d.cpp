#include "Object3d.h"
#include "Object3dBase.h"
#include "Math.h"
#include "TextureManager.h"
#include "Model.h"
#include "ModelManager.h"
#include <fstream>

//初期化
void Object3d::Initialize(Object3dBase* object3dBase) {
	this->object3dBase_ = object3dBase;
	this->camera = object3dBase_->GetDefaultCamera();
	TransformationCreate();
	DirectionalLightCreate();
	CameraDataCreate();
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}
//更新
void Object3d::Update() {
	//TransformからWorldMatrixを作る
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewProjectionMatrix);
	}
	else {
		worldViewProjectionMatrix = worldMatrix;
	}

	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

	cameraData->worldPosition = cameraTransform_.translate;
}
//描画
void Object3d::Draw() {

	//座標変換行列CBufferの場所を設定
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	//Lighting
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(3, DirectionalLightResource->GetGPUVirtualAddress());
	//camera
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());

	//3Dモデルが割り当てられていたら描画する
	if (model_) {
		model_->Draw();
	}
}
//setter
void Object3d::SetModel(const std::string& filePath) {
	//モデルを検索してセットする
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}
//座標変換行列データ作成
void Object3d::TransformationCreate() {
	//リソースを作る
	transformationMatrixResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスの取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}
//平行光源データ作成
void Object3d::DirectionalLightCreate() {
	DirectionalLightResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレスと取得
	DirectionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight));
	//デフォルト値はとりあえず以下のようにしておく
	directionalLight->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight->direction = { 0.0f,-1.0f,0.0f };
	directionalLight->intensity = 1.0f;
}
//カメラデータ作成
void Object3d::CameraDataCreate() {
	cameraResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(CameraForGPU));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
	cameraData->worldPosition;
}