#include "ParticleManager.h"
#include "Math.h"
#include <assert.h>
#include <numbers>

void ParticleManager::Initialize(DirectXBase*directxBase,SrvManager*srvManager) {
	this->directxBase_ = directxBase;
	this->srvManager_ = srvManager;
	randomEngine.seed(seedGenerator());
	//ルートシグネチャ
	GenerateRootSignature();
	//グラフィックスパイプライン
	GenerategraphicsPipeline();
	/*
	//左下
	vertexData[0].position = { 0.0f,1.0f,0.0f,1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	//左上
	vertexData[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.0f,0.0f };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	//右下
	vertexData[2].position = { 1.0f,1.0f,0.0f,1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	//右上
	vertexData[3].position = { 1.0f,0.0f,0.0f,1.0f };
	vertexData[3].texcoord = { 1.0f,0.0f };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };
	*/
	//頂点データ
	VertexDataCreate();

}

void ParticleManager::Update() {
	Matrix4x4 cameraMatrix = camera_->GetWorldMatrix();
	Matrix4x4 backToFrontMatrix = Math::MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Math::Multiply(backToFrontMatrix, cameraMatrix);
	//ビュー行列
	Matrix4x4 viewMatrix = camera_->GetViewMatrix();
	//プロジェクション行列
	Matrix4x4 projectionMatrix = camera_->GetProjectionMatrix();
	//ビュープロジェクション行列
	Matrix4x4 viewprojectionMatrix = camera_->GetViewProjectionMatrix();
	//trueなら使う
	if (useBillboard) {
		billboardMatrix.m[3][0] = 0.0f;
		billboardMatrix.m[3][1] = 0.0f;
		billboardMatrix.m[3][2] = 0.0f;
	}
	//falseなら単位行列
	else if (!useBillboard) {
		billboardMatrix = Math::MakeIdentity4x4();
	}
	for (auto& ParticleGroups : particleGroups) {
		for (std::list<Particle>::iterator particleIterator = ParticleGroups.second.particles.begin();
			particleIterator != ParticleGroups.second.particles.end();) {
			//寿命に達したらグループから外す
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = ParticleGroups.second.particles.erase(particleIterator);
				continue;
			}
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			if (ParticleGroups.second.kNumInstance < kNumMaxInstance) {
				//行列計算
				Matrix4x4 scaleMatrix = Math::MakeScaleMatrix((*particleIterator).transform.scale);
				Matrix4x4 translateMatrix = Math::MakeTranslateMatrix((*particleIterator).transform.translate);
				Matrix4x4 worldMatrix = Math::Multiply(scaleMatrix, Math::Multiply(billboardMatrix, translateMatrix));
				Matrix4x4 worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewprojectionMatrix);
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].WVP = worldViewProjectionMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].World = worldMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].color;
				//速度を適用
				(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * kDeltaTime;
				(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * kDeltaTime;
				(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * kDeltaTime;
				//インスタンシング用データ書き込み
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].WVP = worldViewProjectionMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].World = worldMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].color.w = alpha;
				++ ParticleGroups.second.kNumInstance;
			}
			++particleIterator;
		}
	}
}
/*
// ルートシグネチャを設定
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// パイプラインステートを設定
	commandList->SetPipelineState(graphicsPipelineState.Get());

	// プリミティブトポロジ（描画形状）を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV (Vertex Buffer View)を設定
	commandList->IASetVertexBuffers(0, 1, &vertexBufferView);

	// SrvManagerのインスタンスを取得
	SrvManager* srvManager = SrvManager::GetInstance();

	// 全てのパーティクルグループについて処理を行う
	for (auto& group : particleGroups) {
		if (group.second.instanceCount == 0) continue; // インスタンスが無い場合はスキップ

		//マテリアルCBufferの場所を設定
		commandList->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

		// テクスチャのSRVのDescriptorTableを設定
		commandList->SetGraphicsRootDescriptorTable(2, srvManager->GetGPUDescriptorHandle(group.second.srvIndex));

		// インスタンシングデータのSRVのDescriptorTableを設定
		commandList->SetGraphicsRootDescriptorTable(1, srvManager->GetGPUDescriptorHandle(group.second.instancingSrvIndex));

		// Draw Call (インスタンシング描画)
		commandList->DrawInstanced(6, group.second.instanceCount, 0, 0);

		// インスタンスカウントをリセット
		group.second.instanceCount = 0;
	}
}
*/
void ParticleManager::Draw() {
	//ルートシグネチャ
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(rootSignature.Get());
	//PSO設定

	//描画形状設定

	//VBV設定
}

//パーティクルグループの生成
void ParticleManager::CreateparticleGroup(const std::string name, const std::string textureFilePath) {
	//登録済みの名前かチェック
	assert(particleGroups.find(name) == particleGroups.end());
	ParticleGroup newParticle;
	//テクスチャファイルパスを設定
	newParticle.materialData.textureFilePath = textureFilePath;
	//テクスチャを読み込む
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	//マテリアルデータにテクスチャSRVインデックスを記録
	newParticle.materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
	//インスタンシング用のリソース生成
	newParticle.instancingResource = directxBase_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	//アドレス取得
	ParticleForGPU* instancingData = nullptr;
	newParticle.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newParticle.instancingData));
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		instancingData[index].WVP = Math::MakeIdentity4x4();
		instancingData[index].World = Math::MakeIdentity4x4();
		instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//SRV生成
	newParticle.SRVIndex = srvManager_->Allccate();
	srvManager_->CreateSRVforStructuredBuffer(newParticle.SRVIndex, newParticle.
		instancingResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
}
//ルートシグネチャの作成
void ParticleManager::GenerateRootSignature() {
	D3D12_ROOT_SIGNATURE_DESC descriptitonRootSignature{};
	descriptitonRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

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
	rootParameter[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	rootParameter[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameter[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameter[3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

	descriptitonRootSignature.pParameters = rootParameter;
	descriptitonRootSignature.NumParameters = _countof(rootParameter);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
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
void ParticleManager::GenerategraphicsPipeline() {
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
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = directxBase_->CompileShader(L"resources/shaders/Object3D.VS.hlsl",
		L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = directxBase_->CompileShader(L"resources/shaders/Object3D.ps.hlsl",
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
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
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
//頂点データ作成
void ParticleManager::VertexDataCreate() {
	ModelData modelData;
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = "./resources/uvChecker.png";
	//リソースを作る
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}