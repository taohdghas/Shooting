#include "Object3d.h"
#include "Object3dBase.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "Model.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include <fstream>
#include <numbers>

// 初期化
void Object3d::Initialize(Object3dBase* object3dBase) {
	this->object3dBase_ = object3dBase;
	this->camera = object3dBase_->GetDefaultCamera();

	// 各種バッファリソースの生成（座標変換・ライト・カメラなど）
	TransformationCreate();
	DirectionalLightCreate();
	CameraDataCreate();
	PointLightCreate();
	SpotLightCreate();

	// Transformの初期化（スケール・回転・平行移動）
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
}

// 更新
void Object3d::Update() {
	modelData_ = model_->GetModelData();

	// モデルにライティング有効・無効を反映
	model_->SetEnableLighting(enableLighting);

	// Transform からワールド行列を作成
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;

	// カメラが有効な場合はViewProjectionを掛け合わせる
	if (camera) {
		const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		worldViewProjectionMatrix = worldMatrix;
	}

	// モデルノードのローカル行列を考慮して最終行列をセット
	transformationMatrixData->WVP = Math::Multiply(modelData_.rootNode.localMatrix, worldViewProjectionMatrix);
	transformationMatrixData->World = Math::Multiply(modelData_.rootNode.localMatrix, worldMatrix);
	transformationMatrixData->WorldInverseTranspose = Math::Transpose(Math::Inverse(worldMatrix));

	// カメラ位置をGPUに転送
	cameraData->worldPosition = cameraTransform_.translate;
}

// 描画
void Object3d::Draw() {
	// 各種リソースをコマンドリストにバインド（描画準備）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootSignature(object3dBase_->GetRootSignature());
	object3dBase_->GetDxBase()->Getcommandlist()->SetPipelineState(object3dBase_->GetGraphicsPipelineState());

	// Transform（行列系CBuffer）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());

	// DirectionalLight（平行光源）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(3, DirectionalLightResource->GetGPUVirtualAddress());

	// Camera（カメラデータ）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(4, cameraResource->GetGPUVirtualAddress());

	// PointLight（点光源）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(5, PointLightResource->GetGPUVirtualAddress());

	// SpotLight（スポットライト）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(6, SpotLightResource->GetGPUVirtualAddress());

	// 環境マップ（スカイボックステクスチャ）
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU("resources/skybox/vz_classic_land_cubemap_ue.dds"));

	// モデルが割り当てられている場合のみ描画
	if (model_) {
		model_->Draw();
	}
}

// デバッグ更新（ImGuiによるライト調整）
void Object3d::DebugUpdate() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Light")) {
		ImGui::ColorEdit4("Light", &directionalLight->color.x);
		ImGui::SliderFloat3("Light", &directionalLight->direction.x, -2.0f, 2.0f);
		directionalLight->direction = Math::Normalize(directionalLight->direction);
		ImGui::DragFloat("LightIntensity", &directionalLight->intensity, 0.01f);

		ImGui::DragFloat3("PointLightPosition", &pointLight->position.x, 0.01f);
		ImGui::DragFloat("PointLightIntensity", &pointLight->intensity, 0.01f);
		ImGui::DragFloat("PointLightRadius", &pointLight->radius, 0.01f);
		ImGui::DragFloat("PointLightDecay", &pointLight->decay, 0.01f);

		ImGui::DragFloat3("SpotLightPosition", &spotLight->position.x, 0.01f);
		ImGui::DragFloat("SpotLightIntensity", &spotLight->intensity, 0.01f);
		ImGui::DragFloat("SpotLightcosFalloffStart", &spotLight->cosFalloffStart, 0.01f);
		ImGui::TreePop();
	}
#endif 
}

// setter
void Object3d::SetModel(const std::string& filePath) {
	// 指定ファイルパスのモデルを検索してセット
	model_ = ModelManager::GetInstance()->FindModel(filePath);
}

void Object3d::SetColor(const Vector4& color) {
	if (model_) {
		model_->SetColor(color);
	}
}

// 座標変換行列データ作成
void Object3d::TransformationCreate() {
	// Transform行列用のCBufferリソースを生成
	transformationMatrixResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));

	// 書き込み先アドレスを取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	// 初期値として単位行列を設定
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
	transformationMatrixData->WorldInverseTranspose = Math::MakeIdentity4x4();
}

// 平行光源データ作成
void Object3d::DirectionalLightCreate() {
	DirectionalLightResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(DirectionalLight));
	DirectionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight));

	// 初期設定（白色・下向き・照度1.0）
	directionalLight->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight->direction = { 0.0f,-1.0f,0.0f };
	directionalLight->intensity = 1.0f;
	directionalLight->enable = 0;
}

// カメラデータ作成
void Object3d::CameraDataCreate() {
	cameraResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(CameraForGPU));
	cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

	// カメラの位置情報を格納（初期値は構造体側のデフォルト）
	cameraData->worldPosition;
}

// ポイントライトデータ作成
void Object3d::PointLightCreate() {
	PointLightResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(PointLight));
	PointLightResource->Map(0, nullptr, reinterpret_cast<void**>(&pointLight));

	// 初期設定（白色・原点付近・半径10・減衰2）
	pointLight->color = { 1.0f,1.0f,1.0f };
	pointLight->intensity = 1.0f;
	pointLight->position = { 0.0f,2.0f,0.0f };
	pointLight->radius = 10.0f;
	pointLight->decay = 2.0f;
	pointLight->enable = 0;
}

// スポットライトデータ作成
void Object3d::SpotLightCreate() {
	SpotLightResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(SpotLight));
	SpotLightResource->Map(0, nullptr, reinterpret_cast<void**>(&spotLight));

	// 初期設定（白色・特定位置・下向き・円錐範囲設定）
	spotLight->color = { 1.0f,1.0f,1.0f };
	spotLight->position = { 2.0f,1.25f,0.0f };
	spotLight->distance = 7.0f;
	spotLight->direction = Math::Normalize({ -1.0f,-1.0f,0.0f });
	spotLight->intensity = 4.0f;
	spotLight->decay = 2.0f;
	spotLight->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
	spotLight->cosFalloffStart = 1.0f;
	spotLight->enable = 0;
}
