#pragma once
#include "DirectXBase.h"

class SpriteBase
{
public:
	//初期化
	void Initialize(DirectXBase*directxBase);
	//共通描画設定
	void DrawBaseSet();

	DirectXBase* GetDxBase()const { return directxBase_; }
private:
	HRESULT hr;

	//DirectXBaseのポインタ
	DirectXBase* directxBase_;
	//ルートシグネチャの作成
	void RootSignatureGenerate();
	//グラフィックスパイプラインの生成
	void graphicsPipelineGenerate();
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//inputlayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};

