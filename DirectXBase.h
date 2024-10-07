#pragma once
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"
#include <dxcapi.h>

class DirectXBase
{
public:
	//初期化
	void Initialize(WindowsAPI* windowsAPI);
	//デスクリプタヒープを生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisivle);
	//SRVの指定番号のCPUデスクリプタハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	//SRVの指定番号のGPUデスクリプタハンドル取得
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);
private:
	//デバイスの初期化
	void DeviceInitialize();
	//コマンド関連の初期化
	void CommandInitialize();
	//スワップチェーンの生成
	void SwapchainGenerate();
	//深度バッファの生成
	void DepthbufferGenerate();
	//各種デスクリプタヒープの生成
	void DescriptorheapGenerate();
	//レンダーターゲットビューの初期化
	void RendertargetviewInitialize();
	//深度ステンシルビューの初期化
	void DepthstencilviewInitialize();
	//フェンスの初期化
	void FenceInitialize();
	//ビューポート矩形の初期化
	void ViewportInitialize();
	//シザリング矩形の初期化
	void ScissorrectInitialize();
	//DXCコンパイラの生成
	void DXCcompilerInitialize();
	//Imguiの初期化
	void ImguiInitialize();
	//DirectX12デバイス
	Microsoft::WRL::ComPtr<ID3D12Device>device;
	//DXGIファクトリ
	Microsoft::WRL::ComPtr<IDXGIFactory7>dxgiFactory;
	//コマンドアロケータ
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
	//コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
	//コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	//IDXGISwapChain4
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;
	//Resource
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;
	//DescriptorSizeSRVを取得
	uint32_t descriptorSizeSRV;
	//DescriptorSizeRTVを取得
	uint32_t descriptorSizeRTV;
	//DescriptorSizeDSVを取得
	uint32_t descriptorSizeDSV;
	//RTV用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;
	//SRV用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap;
	//DSV用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	///深度ステンシルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> depthStencilResource;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	//DXCユーティリティ
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils;
	//DXCコンパイラ
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	//デフォルトインクルードハンドラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler>includeHandler;
	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	//WindowsAPI
	WindowsAPI* windowsAPI = nullptr;
	//CPUデスクリプタハンドル取得関数
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//GPUデスクリプタハンドル取得関数
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//スワップチェインリソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;
};

