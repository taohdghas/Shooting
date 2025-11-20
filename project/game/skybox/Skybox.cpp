#include "Skybox.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "Logger.h"
#include "MyMath.h"

// Skyboxの初期化処理
void Skybox::Initialize(std::string textureFilePath) {
	directxBase_ = DirectXBase::GetInstance();
	pso_ = std::make_unique<PipelineStateObject>();
	pso_->Initialize(directxBase_);
	textureFilePath_ = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(textureFilePath_);
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
	//transform_ = { {1000.0f,1000.0f,1000.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

// 毎フレームの更新処理
void Skybox::Update() {
	// ワールド行列の作成
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	// カメラが存在する場合はViewProjectionを掛ける
	if (CameraManager::GetInstance()) {
		const Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		worldViewProjectionMatrix = worldMatrix;
	}
	// 行列データをCBufferへ書き込み
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}

// 描画処理
void Skybox::Draw() {
	// ルートシグネチャ設定
	directxBase_->GetCommandList()->SetGraphicsRootSignature(pso_->GetSkyBoxRootSignature());
	// PSO設定
	directxBase_->GetCommandList()->SetPipelineState(pso_->GetSkyBoxGraphicsPipelineState());
	// 描画形状設定
	directxBase_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// VBV設定
	directxBase_->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックス設定
	directxBase_->GetCommandList()->IASetIndexBuffer(&indexBufferView);
	// マテリアルCBufferの場所を設定
	directxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// テクスチャのDescriptorTableを設定
	directxBase_->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));
	// インスタンシングデータのSRVのDescriptorTableを設定
	directxBase_->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// 描画コマンド発行
	directxBase_->GetCommandList()->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);
}

// インデックスデータ作成
void Skybox::CreateIndex() {
	indices = {
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		8, 9,10,10,9,11,
		12,13,14,14,13,15,
		16,17,18,18,17,19,
		20,21,22,22,21,23,
	};

	// インデックス用リソース作成
	indexResource = directxBase_->CreateBufferResource(sizeof(uint32_t) * indices.size());
	// バッファビュー設定
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indices.size());
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	// リソースへデータ書き込み
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, indices.data(), sizeof(uint32_t) * indices.size());
}

// マテリアル作成
void Skybox::CreateMaterial() {
	// マテリアル用リソース作成
	materialResource = directxBase_->CreateBufferResource(sizeof(Material));
	// 書き込み用アドレス取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	// マテリアル初期値設定
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	materialData->enableLighting = false;
	materialData->uvTransform = Math::MakeIdentity4x4();
}

// 座標変換行列データ作成
void Skybox::CreateTransformation() {
	// 行列用リソース作成
	transformationMatrixResource = directxBase_->CreateBufferResource(sizeof(TransformationMatrix));
	// 書き込み用アドレス取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列で初期化
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}

// 頂点データ作成
void Skybox::CreateVertex() {
	vertices.resize(24);
	// 右面
	vertices[0].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[1].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices[3].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	// 左面
	vertices[4].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[5].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[6].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[7].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	// 前面
	vertices[8].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[9].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices[10].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[11].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	// 後面
	vertices[12].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[13].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	// 上面
	vertices[16].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[17].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[18].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[19].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	// 下面
	vertices[20].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[21].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	vertices[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertices[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	// 頂点用リソース作成
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * vertices.size());
	// バッファビュー設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	// リソースへデータ書き込み
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, vertices.data(), sizeof(VertexData) * vertices.size());
}
