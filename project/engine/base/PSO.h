#pragma once
#include "DirectXBase.h"

class PSO
{
public:
	//初期化
	void Initialize(DirectXBase*directBase);
	//ルートシグネチャ作成
	void CreateRootSignature();
	//グラフィックスパイプライン作成
	void CreatePipelineState();
public:
	ID3D12RootSignature* GetRootSignature()const { return rootSignature.Get(); }
	ID3D12PipelineState* GetGraphicsPipelineState()const { return graphicsPipelineState.Get(); }
private:
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;

	HRESULT hr;
	DirectXBase* directxBase_ = nullptr;
};

