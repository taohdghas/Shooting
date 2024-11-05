#pragma once
#include "DirectXBase.h"

//3Dオブジェクト共通部
class Object3dBase
{
public:
	//初期化
	void Initialize(DirectXBase*directxBase);
	//ルートシグネチャの作成
	void GenerateRootSignature();
	//グラフィックスパイプラインの生成
	void GenerategraphicsPipeline();
	//共通描画設定
	void DrawBaseSet();
	///getter///
	DirectXBase* GetDxBase()const { return directxBase_; }

private:
	HRESULT hr;
	//DirectXBaseのポインタ
	DirectXBase* directxBase_;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//inputlayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};

