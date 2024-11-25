#pragma once
#include "DirectXBase.h"

//SRV管理
class SrvManager
{
public:
	//初期化
	void Initialize(DirectXBase* directxBase);
	//SRV生成(テクスチャ用)
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels);
	//SRV生成(Structured Buffer用)
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	//ヒープセットコマンド
	void PreDraw();
	//SRVセットコマンド
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);
	//確保可能かチェック
	bool Securedcheck();
	uint32_t Allccate();
	//CPUハンドル計算
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	//GPUハンドル計算
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
public:
	//最大SRV数(最大テクスチャ数)
	static const uint32_t kMaxCount;
private:
	DirectXBase* directxBase = nullptr;
	//SRV用DescriptorSizeを取得
	uint32_t descriptorSize;
	//次に使用するSRVindex
	uint32_t useIndex = 0;
	//SRV用デスクリプターヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;
};

