#include "Object3d.h"
#include "Object3dBase.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "Model.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include <fstream>
#include <numbers>

namespace MyEngine {

	// 初期化
	void Object3d::Initialize(Object3dBase* object3dBase) {
		this->object3d_base_ = object3dBase;
		this->camera_ = object3d_base_->GetDefaultCamera();

		// 各種バッファリソースの生成（座標変換・ライト・カメラなど）
		TransformationCreate();
		DirectionalLightCreate();
		CameraDataCreate();
		PointLightCreate();
		SpotLightCreate();

		// Transformの初期化（スケール・回転・平行移動）
		transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		camera_transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	}

	// 更新
	void Object3d::Update() {
		model_data_ = model_->GetModelData();

		// モデルにライティング有効・無効を反映
		model_->SetEnableLighting(enable_lighting_);

		// Transform からワールド行列を作成
		Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		Matrix4x4 worldViewProjectionMatrix;

		// カメラが有効な場合はViewProjectionを掛け合わせる
		if (camera_) {
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewProjectionMatrix);
		} else {
			worldViewProjectionMatrix = worldMatrix;
		}

		// モデルノードのローカル行列を考慮して最終行列をセット
		transformation_matrix_data_->WVP = Math::Multiply(model_data_.rootNode.localMatrix, worldViewProjectionMatrix);
		transformation_matrix_data_->World = Math::Multiply(model_data_.rootNode.localMatrix, worldMatrix);
		transformation_matrix_data_->WorldInverseTranspose = Math::Transpose(Math::Inverse(worldMatrix));

		// カメラ位置をGPUに転送
		camera_data_->worldPosition = camera_transform_.translate;
	}

	// 描画
	void Object3d::Draw() {
		// 各種リソースをコマンドリストにバインド（描画準備）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootSignature(object3d_base_->GetRootSignature());
		object3d_base_->GetDxBase()->GetCommandList()->SetPipelineState(object3d_base_->GetGraphicsPipelineState());

		// Transform（行列系CBuffer）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformation_matrix_resource_->GetGPUVirtualAddress());

		// DirectionalLight（平行光源）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(3, directional_light_resource_->GetGPUVirtualAddress());

		// Camera（カメラデータ）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(4, camera_resource_->GetGPUVirtualAddress());

		// PointLight（点光源）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(5, point_light_resource_->GetGPUVirtualAddress());

		// SpotLight（スポットライト）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(6, spot_light_resource_->GetGPUVirtualAddress());

		// 環境マップ（スカイボックステクスチャ）
		object3d_base_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(7, TextureManager::GetInstance()->GetSrvHandleGPU("resources/skybox/vz_classic_land_cubemap_ue.dds"));

		// モデルが割り当てられている場合のみ描画
		if (model_) {
			model_->Draw();
		}
	}

	// デバッグ更新（ImGuiによるライト調整）
	void Object3d::DebugUpdate() {
#ifdef USE_IMGUI
		if (ImGui::TreeNode("Light")) {
			ImGui::ColorEdit4("Light", &directional_light_->color.x);
			ImGui::SliderFloat3("Light", &directional_light_->direction.x, -2.0f, 2.0f);
			directional_light_->direction = Math::Normalize(directional_light_->direction);
			ImGui::DragFloat("LightIntensity", &directional_light_->intensity, 0.01f);

			ImGui::DragFloat3("PointLightPosition", &point_light_->position.x, 0.01f);
			ImGui::DragFloat("PointLightIntensity", &point_light_->intensity, 0.01f);
			ImGui::DragFloat("PointLightRadius", &point_light_->radius, 0.01f);
			ImGui::DragFloat("PointLightDecay", &point_light_->decay, 0.01f);

			ImGui::DragFloat3("SpotLightPosition", &spot_light_->position.x, 0.01f);
			ImGui::DragFloat("SpotLightIntensity", &spot_light_->intensity, 0.01f);
			ImGui::DragFloat("SpotLightcosFalloffStart", &spot_light_->cosFalloffStart, 0.01f);
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
		transformation_matrix_resource_ = object3d_base_->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));

		// 書き込み先アドレスを取得
		transformation_matrix_resource_->Map(0, nullptr, reinterpret_cast<void**>(&transformation_matrix_data_));

		// 初期値として単位行列を設定
		transformation_matrix_data_->WVP = Math::MakeIdentity4x4();
		transformation_matrix_data_->World = Math::MakeIdentity4x4();
		transformation_matrix_data_->WorldInverseTranspose = Math::MakeIdentity4x4();
	}

	// 平行光源データ作成
	void Object3d::DirectionalLightCreate() {
		directional_light_resource_ = object3d_base_->GetDxBase()->CreateBufferResource(sizeof(DirectionalLight));
		directional_light_resource_->Map(0, nullptr, reinterpret_cast<void**>(&directional_light_));

		// 初期設定（白色・下向き・照度1.0）
		directional_light_->color = { 1.0f,1.0f,1.0f,1.0f };
		directional_light_->direction = { 0.0f,-1.0f,0.0f };
		directional_light_->intensity = 1.0f;
		directional_light_->enable = 0;
	}

	// カメラデータ作成
	void Object3d::CameraDataCreate() {
		camera_resource_ = object3d_base_->GetDxBase()->CreateBufferResource(sizeof(CameraForGPU));
		camera_resource_->Map(0, nullptr, reinterpret_cast<void**>(&camera_data_));

		// カメラの位置情報を格納（初期値は構造体側のデフォルト）
		camera_data_->worldPosition;
	}

	// ポイントライトデータ作成
	void Object3d::PointLightCreate() {
		point_light_resource_ = object3d_base_->GetDxBase()->CreateBufferResource(sizeof(PointLight));
		point_light_resource_->Map(0, nullptr, reinterpret_cast<void**>(&point_light_));

		// 初期設定（白色・原点付近・半径10・減衰2）
		point_light_->color = { 1.0f,1.0f,1.0f };
		point_light_->intensity = 1.0f;
		point_light_->position = { 0.0f,2.0f,0.0f };
		point_light_->radius = 10.0f;
		point_light_->decay = 2.0f;
		point_light_->enable = 0;
	}

	// スポットライトデータ作成
	void Object3d::SpotLightCreate() {
		spot_light_resource_ = object3d_base_->GetDxBase()->CreateBufferResource(sizeof(SpotLight));
		spot_light_resource_->Map(0, nullptr, reinterpret_cast<void**>(&spot_light_));

		// 初期設定（白色・特定位置・下向き・円錐範囲設定）
		spot_light_->color = { 1.0f,1.0f,1.0f };
		spot_light_->position = { 2.0f,1.25f,0.0f };
		spot_light_->distance = 7.0f;
		spot_light_->direction = Math::Normalize({ -1.0f,-1.0f,0.0f });
		spot_light_->intensity = 4.0f;
		spot_light_->decay = 2.0f;
		spot_light_->cosAngle = std::cos(std::numbers::pi_v<float> / 3.0f);
		spot_light_->cosFalloffStart = 1.0f;
		spot_light_->enable = 0;
	}
}