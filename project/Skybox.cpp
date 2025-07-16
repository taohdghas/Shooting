#include "Skybox.h"
#include "Logger.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "Math.h"

//初期化
void Skybox::Initialize(std::string textureFilePath) {
	directxBase_ = DirectXBase::GetInstance();
	textureFilePath_ = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(textureFilePath_);
	//頂点データ作成
	CreateVertex();
	//index作成
	CreateIndex();
	//マテリアル作成
	CreateMaterial();
	//座標変換行列データ作成
	CreateTransformation();
	//グラフィックスパイプラインの生成
	GenerateGraphicsPipeline();
	transform_ = { {1000.0f,1000.0f,1000.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
}

//更新
void Skybox::Update() {
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 worldViewProjectionMatrix;
	if (CameraManager::GetInstance()) {
		const Matrix4x4& viewProjectionMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
		worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewProjectionMatrix);
	} else {
		worldViewProjectionMatrix = worldMatrix;
	}
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}

//描画
void Skybox::Draw() {
	//ルートシグネチャ
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(rootSignature.Get());
	//PSO設定
	directxBase_->Getcommandlist()->SetPipelineState(graphicsPipelineState.Get());
	//描画形状設定
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//VBV設定
	directxBase_->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//index設定
	directxBase_->Getcommandlist()->IASetIndexBuffer(&indexBufferView);
	//マテリアルCBufferの場所を設定
	directxBase_->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//テクスチャのDescriptorTableを設定
	directxBase_->Getcommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(textureFilePath_));
	//インスタンシングデータのSRVのDescriptorTableを設定
	directxBase_->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	//描画
	directxBase_->Getcommandlist()->DrawIndexedInstanced(static_cast<UINT>(indices.size()), 1, 0, 0, 0);

}

//index作成
void Skybox::CreateIndex() {

	indices = {
		0,1,2,2,1,3,
		4,5,6,6,5,7,
		8, 9,10,10,9,11,
		12,13,14,14,13,15,
		16,17,18,18,17,19,
		20,21,22,22,21,23,
	};

	//リソースを作る
	indexResource = directxBase_->CreateBufferResource(sizeof(uint32_t) * indices.size());
	//リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = static_cast<UINT>(sizeof(uint32_t) * indices.size());
	//インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込むためのアドレスを取得
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	std::memcpy(indexData, indices.data(), sizeof(uint32_t) * indices.size());
}

//マテリアル作成
void Skybox::CreateMaterial() {
	//リソースを作る
	materialResource = directxBase_->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスと取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//マテリアルデータの初期値を書き込む
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	//Lighting
	materialData->enableLighting = false;
	//UVTransform行列を単位行列で初期化
	materialData->uvTransform = Math::MakeIdentity4x4();
}

//座標変換行列データ作成
void Skybox::CreateTransformation() {
	//リソースを作る
	transformationMatrixResource = directxBase_->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスの取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}

//頂点データ作成
void Skybox::CreateVertex() {

	vertices.resize(24);
	//右面
	vertices[0].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[1].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices[3].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	//左面
	vertices[4].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[5].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[6].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[7].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	//前面
	vertices[8].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[9].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	vertices[10].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[11].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	//後面
	vertices[12].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[13].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	//上面
	vertices[16].position = { -1.0f,  1.0f, -1.0f, 1.0f };
	vertices[17].position = { 1.0f,  1.0f, -1.0f, 1.0f };
	vertices[18].position = { -1.0f,  1.0f,  1.0f, 1.0f };
	vertices[19].position = { 1.0f,  1.0f,  1.0f, 1.0f };
	//下面
	vertices[20].position = { -1.0f, -1.0f,  1.0f, 1.0f };
	vertices[21].position = { 1.0f, -1.0f,  1.0f, 1.0f };
	vertices[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertices[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	//リソースを作る
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) *vertices.size());
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, vertices.data(), sizeof(VertexData) * vertices.size());
}

//ルートシグネチャの作成
void Skybox::GenerateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC descriptitonRootSignature{};
	descriptitonRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	rootParameter[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[0].Descriptor.ShaderRegister = 0;

	rootParameter[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameter[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameter[1].Descriptor.ShaderRegister = 0;

	rootParameter[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameter[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameter[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameter[2].DescriptorTable.NumDescriptorRanges = 1;

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameter[3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

	descriptitonRootSignature.pParameters = rootParameter;
	descriptitonRootSignature.NumParameters = _countof(rootParameter);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptitonRootSignature.pStaticSamplers = staticSamplers;
	descriptitonRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&descriptitonRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	// バイナリを元に生成
	hr = directxBase_->Getdevice()->CreateRootSignature(0,
		signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));
}

//グラフィックスパイプラインの生成
void Skybox::GenerateGraphicsPipeline() {
	GenerateRootSignature();
	// InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	// BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;

	// RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//カリングしない(裏面も表示)
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// shaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = directxBase_->CompileShader(L"resources/shaders/Skybox.VS.hlsl",
		L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = directxBase_->CompileShader(L"resources/shaders/Skybox.PS.hlsl",
		L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),
	vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),
	pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	// 書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	// 利用するトポロジ（形状）タイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	// どのように画面に色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの昨日を有効化にする
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	// 比較関数はLessEqual.つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 実際に生成
	hr = directxBase_->Getdevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}