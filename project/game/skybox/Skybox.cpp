#include "Skybox.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include "MyMath.h"

// Skyboxの初期化処理
void Skybox::Initialize(std::string texture_file_path) {
	directx_base_ = DirectXBase::GetInstance();
	pso_ = std::make_unique<PipelineStateObject>();
	pso_->Initialize(directx_base_);
	texture_file_path_ = texture_file_path;
	TextureManager::GetInstance()->LoadTexture(texture_file_path_);
	// 頂点データ作成
	CreateVertex();
	// インデックスデータ作成
	CreateIndex();
	// マテリアル作成
	CreateMaterial();
	// 座標変換行列データ作成
	CreateTransformation();
	// グラフィックスパイプライン作成
	pso_->CreateSkyBoxPipelineState();
	// Transform初期値設定
	transform_ = { {1000.0f,1000.0f,1000.0f},{5.0f,25.3f,0.0f},{0.0f,150.0f,0.0f} };
}

// 毎フレームの更新処理
void Skybox::Update() {
	// ワールド行列の作成
	Matrix4x4 world_matrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 world_view_projection_matrix;
	// カメラが存在する場合はViewProjectionを掛ける
	if (CameraManager::GetInstance()) {
		const Matrix4x4& view_projection_matrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
		world_view_projection_matrix = Math::Multiply(world_matrix, view_projection_matrix);
	} else {
		world_view_projection_matrix = world_matrix;
	}
	// 行列データをCBufferへ書き込み
	transformation_matrix_data_->WVP = world_view_projection_matrix;
	transformation_matrix_data_->World = world_matrix;
}

// 描画処理
void Skybox::Draw() {
	// ルートシグネチャ設定
	directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetSkyBoxRootSignature());
	// PSO設定
	directx_base_->GetCommandList()->SetPipelineState(pso_->GetSkyBoxGraphicsPipelineState());
	// 描画形状設定
	directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// VBV設定
	directx_base_->GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
	// インデックス設定
	directx_base_->GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
	// マテリアルCBufferの場所を設定
	directx_base_->GetCommandList()->SetGraphicsRootConstantBufferView(0, material_resource_->GetGPUVirtualAddress());
	// テクスチャのDescriptorTableを設定
	directx_base_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(texture_file_path_));
	// インスタンシングデータのSRVのDescriptorTableを設定
	directx_base_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformation_matrix_resource_->GetGPUVirtualAddress());
	// 描画コマンド発行
	directx_base_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(indices_.size()), 1, 0, 0, 0);
}

// インデックスデータ作成
void Skybox::CreateIndex() {
	indices_ = {
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		8, 9,10,10,9,11,
		12,13,14,14,13,15,
		16,17,18,18,17,19,
		20,21,22,22,21,23,
	};

	// インデックス用リソース作成
	index_resource_ = directx_base_->CreateBufferResource(sizeof(uint32_t) * indices_.size());
	// バッファビュー設定
	index_buffer_view_.BufferLocation = index_resource_->GetGPUVirtualAddress();
	index_buffer_view_.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indices_.size());
	index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;
	// リソースへデータ書き込み
	index_resource_->Map(0, nullptr, reinterpret_cast<void**>(&index_data_));
	std::memcpy(index_data_, indices_.data(), sizeof(uint32_t) * indices_.size());
}

// マテリアル作成
void Skybox::CreateMaterial() {
	// マテリアル用リソース作成
	material_resource_ = directx_base_->CreateBufferResource(sizeof(Material));
	// 書き込み用アドレス取得
	material_resource_->Map(0, nullptr, reinterpret_cast<void**>(&material_data_));
	// マテリアル初期値設定
	material_data_->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	material_data_->enableLighting = false;
	material_data_->uvTransform = Math::MakeIdentity4x4();
}

// 座標変換行列データ作成
void Skybox::CreateTransformation() {
	// 行列用リソース作成
	transformation_matrix_resource_ = directx_base_->CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込み用アドレス取得
	transformation_matrix_resource_->Map(0, nullptr, reinterpret_cast<void**>(&transformation_matrix_data_));
	// 単位行列で初期化
	transformation_matrix_data_->WVP = Math::MakeIdentity4x4();
	transformation_matrix_data_->World = Math::MakeIdentity4x4();
}

// 頂点データ作成
void Skybox::CreateVertex() {
	vertices_.resize(24);
	// 右面
	vertices_[0].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[1].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices_[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices_[3].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	// 左面
	vertices_[4].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices_[5].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[6].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices_[7].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	// 前面
	vertices_[8].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices_[9].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices_[10].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices_[11].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	// 後面
	vertices_[12].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[13].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices_[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	// 上面
	vertices_[16].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[17].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices_[18].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices_[19].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	// 下面
	vertices_[20].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices_[21].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	vertices_[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertices_[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	// 頂点用リソース作成
	vertex_resource_ = directx_base_->CreateBufferResource(sizeof(VertexData) * vertices_.size());
	// バッファビュー設定
	vertex_buffer_view_.BufferLocation = vertex_resource_->GetGPUVirtualAddress();
	vertex_buffer_view_.SizeInBytes = UINT(sizeof(VertexData) * vertices_.size());
	vertex_buffer_view_.StrideInBytes = sizeof(VertexData);
	// リソースへデータ書き込み
	vertex_resource_->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data_));
	std::memcpy(vertex_data_, vertices_.data(), sizeof(VertexData) * vertices_.size());
}
