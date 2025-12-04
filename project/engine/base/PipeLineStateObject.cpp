#include "PipeLineStateObject.h"
#include "DirectXBase.h"
#include "Logger.h"

//初期化
void PipelineStateObject::Initialize(DirectXBase* directXBase) {
	directx_base_ = directXBase;
}
//Object3D用RootSignature
void PipelineStateObject::CreateRootSignature() {
	D3D12_DESCRIPTOR_RANGE descriptorRange[2] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	descriptorRange[1].BaseShaderRegister = 1;
	descriptorRange[1].NumDescriptors = 1;
	descriptorRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[8] = {};
	SetRootParameterCBV(rootParameter[0], 0, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[1], 0, D3D12_SHADER_VISIBILITY_VERTEX);
	SetRootParameterDescriptorTable(rootParameter[2], &descriptorRange[0], 1, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[3], 1, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[4], 2, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[5], 3, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[6], 4, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterDescriptorTable(rootParameter[7], &descriptorRange[1], 1, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	SetStaticSampler(staticSamplers[0], D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	CreateRootSignatureCommon(rootParameter, _countof(rootParameter), descriptorRange, _countof(descriptorRange), staticSamplers, _countof(staticSamplers), root_signature_);
}
//Object3D用PipelineState
void PipelineStateObject::CreatePipelineState() {
	CreateRootSignature();

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{ inputElementDescs, _countof(inputElementDescs) };

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	CreateGraphicsPipelineStateCommon(
		root_signature_.Get(),
		inputLayoutDesc,
		blendDesc,
		rasterizerDesc,
		depthStencilDesc,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		L"resources/shaders/Object3D.VS.hlsl", L"vs_6_0",
		L"resources/shaders/Object3D.ps.hlsl", L"ps_6_0",
		graphics_pipeline_state_
	);
}
//Sprite用RootSignature
void PipelineStateObject::CreateSpriteRootSignature() {
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[7] = {};
	SetRootParameterCBV(rootParameter[0], 0, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[1], 0, D3D12_SHADER_VISIBILITY_VERTEX);
	SetRootParameterDescriptorTable(rootParameter[2], descriptorRange, _countof(descriptorRange), D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[3], 1, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[4], 2, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[5], 3, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[6], 4, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	SetStaticSampler(staticSamplers[0], D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	CreateRootSignatureCommon(rootParameter, _countof(rootParameter), descriptorRange, _countof(descriptorRange), staticSamplers, _countof(staticSamplers), sprite_root_signature_);
}
//Sprite用PipelineState
void PipelineStateObject::CreateSpritePipelineState() {
	CreateSpriteRootSignature();

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{ inputElementDescs, _countof(inputElementDescs) };

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	CreateGraphicsPipelineStateCommon(
		sprite_root_signature_.Get(),
		inputLayoutDesc,
		blendDesc,
		rasterizerDesc,
		depthStencilDesc,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		L"resources/shaders/Sprite.VS.hlsl", L"vs_6_0",
		L"resources/shaders/Sprite.ps.hlsl", L"ps_6_0",
		sprite_graphics_pipeline_state_
	);
}
//Particle用RootSignature
void PipelineStateObject::CreateParticleRootSignature() {
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

	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	SetRootParameterCBV(rootParameter[0], 0, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterDescriptorTable(rootParameter[1], descriptorRangeForInstancing, _countof(descriptorRangeForInstancing), D3D12_SHADER_VISIBILITY_VERTEX);
	SetRootParameterDescriptorTable(rootParameter[2], descriptorRange, _countof(descriptorRange), D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[3], 1, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	SetStaticSampler(staticSamplers[0], D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	CreateRootSignatureCommon(rootParameter, _countof(rootParameter), descriptorRange, _countof(descriptorRange), staticSamplers, _countof(staticSamplers), particle_root_signature_);
}
//Particle用PipelineState
void PipelineStateObject::CreateParticlePipelineState() {
	CreateParticleRootSignature();

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{ inputElementDescs, _countof(inputElementDescs) };

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	CreateGraphicsPipelineStateCommon(
		particle_root_signature_.Get(),
		inputLayoutDesc,
		blendDesc,
		rasterizerDesc,
		depthStencilDesc,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		L"resources/shaders/Particle.VS.hlsl", L"vs_6_0",
		L"resources/shaders/Particle.PS.hlsl", L"ps_6_0",
		particle_graphics_pipeline_state_
	);
}
//PostEffect用RootSignature
void PipelineStateObject::CreatePostEffectRootSignature() {
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameter[7] = {};
	SetRootParameterCBV(rootParameter[0], 0, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[1], 0, D3D12_SHADER_VISIBILITY_VERTEX);
	SetRootParameterDescriptorTable(rootParameter[2], descriptorRange, _countof(descriptorRange), D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[3], 1, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[4], 2, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[5], 3, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[6], 4, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	SetStaticSampler(staticSamplers[0], D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	CreateRootSignatureCommon(rootParameter, _countof(rootParameter), descriptorRange, _countof(descriptorRange), staticSamplers, _countof(staticSamplers), post_effect_root_signature_);
}
//PostEffect用PipelineState
void PipelineStateObject::CreatePostEffectPipelineState() {
	CreatePostEffectRootSignature();

	// PostEffectはInputLayout不要
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = false;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	CreateGraphicsPipelineStateCommon(
		post_effect_root_signature_.Get(),
		inputLayoutDesc,
		blendDesc,
		rasterizerDesc,
		depthStencilDesc,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		L"resources/shaders/Fullscreen.VS.hlsl", L"vs_6_0",
		L"resources/shaders/GaussianFilter.ps.hlsl", L"ps_6_0",
		post_effect_graphics_pipeline_state_
	);
}
//SkyBox用RootSignature
void PipelineStateObject::CreateSkyBoxRootSignature() {
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

	D3D12_ROOT_PARAMETER rootParameter[4] = {};
	SetRootParameterCBV(rootParameter[0], 0, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[1], 0, D3D12_SHADER_VISIBILITY_VERTEX);
	SetRootParameterDescriptorTable(rootParameter[2], descriptorRange, 1, D3D12_SHADER_VISIBILITY_PIXEL);
	SetRootParameterCBV(rootParameter[3], 1, D3D12_SHADER_VISIBILITY_PIXEL);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	SetStaticSampler(staticSamplers[0], D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 0, D3D12_SHADER_VISIBILITY_PIXEL);

	CreateRootSignatureCommon(rootParameter, _countof(rootParameter), descriptorRange, _countof(descriptorRange), staticSamplers, _countof(staticSamplers), sky_box_root_signature_);
}
//SkyBox用PipelineState
void PipelineStateObject::CreateSkyBoxPipelineState() {
	CreateSkyBoxRootSignature();

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{ inputElementDescs, _countof(inputElementDescs) };

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	CreateGraphicsPipelineStateCommon(
		sky_box_root_signature_.Get(),
		inputLayoutDesc,
		blendDesc,
		rasterizerDesc,
		depthStencilDesc,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		L"resources/shaders/Skybox.VS.hlsl", L"vs_6_0",
		L"resources/shaders/Skybox.PS.hlsl", L"ps_6_0",
		sky_box_graphics_pipeline_state_
	);
}

void PipelineStateObject::CreateRootSignatureCommon(
	D3D12_ROOT_PARAMETER* rootParameters, UINT numRootParams,
	D3D12_DESCRIPTOR_RANGE* descriptorRanges, UINT numDescriptorRanges,
	D3D12_STATIC_SAMPLER_DESC* staticSamplers, UINT numStaticSamplers,
	Microsoft::WRL::ComPtr<ID3D12RootSignature>& outRootSignature)
{
	D3D12_ROOT_SIGNATURE_DESC desc{};
	desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	desc.pParameters = rootParameters;
	desc.NumParameters = numRootParams;
	desc.pStaticSamplers = staticSamplers;
	desc.NumStaticSamplers = numStaticSamplers;

	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}
	hr = directx_base_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&outRootSignature));
	assert(SUCCEEDED(hr));
}
void PipelineStateObject::CreateGraphicsPipelineStateCommon(
	ID3D12RootSignature* rootSignature,
	const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc,
	const D3D12_BLEND_DESC& blendDesc,
	const D3D12_RASTERIZER_DESC& rasterizerDesc,
	const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc,
	DXGI_FORMAT rtvFormat,
	DXGI_FORMAT dsvFormat,
	const wchar_t* vsPath,
	const wchar_t* vsProfile,
	const wchar_t* psPath,
	const wchar_t* psProfile,
	Microsoft::WRL::ComPtr<ID3D12PipelineState>& outPipelineState)
{
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = directx_base_->CompileShader(vsPath, vsProfile);
	assert(vertexShaderBlob != nullptr);
	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = directx_base_->CompileShader(psPath, psProfile);
	assert(pixelShaderBlob != nullptr);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
	desc.pRootSignature = rootSignature;
	desc.InputLayout = inputLayoutDesc;
	desc.VS = { vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	desc.PS = { pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	desc.BlendState = blendDesc;
	desc.RasterizerState = rasterizerDesc;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = rtvFormat;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.SampleDesc.Count = 1;
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	desc.DepthStencilState = depthStencilDesc;
	desc.DSVFormat = dsvFormat;

	HRESULT hr = directx_base_->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&outPipelineState));
	assert(SUCCEEDED(hr));
}

 void PipelineStateObject::SetRootParameterCBV(D3D12_ROOT_PARAMETER& param, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility) {
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	param.ShaderVisibility = visibility;
	param.Descriptor.ShaderRegister = shaderRegister;
}

// 汎用: RootParameterのDescriptorTable設定
 void PipelineStateObject::SetRootParameterDescriptorTable(D3D12_ROOT_PARAMETER& param, D3D12_DESCRIPTOR_RANGE* range, UINT numRanges, D3D12_SHADER_VISIBILITY visibility) {
	param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	param.ShaderVisibility = visibility;
	param.DescriptorTable.pDescriptorRanges = range;
	param.DescriptorTable.NumDescriptorRanges = numRanges;
}

// 汎用: StaticSamplerの設定
 void PipelineStateObject::SetStaticSampler(D3D12_STATIC_SAMPLER_DESC& sampler, D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV, D3D12_TEXTURE_ADDRESS_MODE addressW, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility) {
	sampler.Filter = filter;
	sampler.AddressU = addressU;
	sampler.AddressV = addressV;
	sampler.AddressW = addressW;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = shaderRegister;
	sampler.ShaderVisibility = visibility;
}