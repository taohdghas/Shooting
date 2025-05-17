#pragma once
#include "Windows.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include "WindowsAPI.h"
#include "Struct.h"
#include <dxcapi.h>
#include <string>
#include <chrono>
#include "externals/DirectXTex/DirectXTex.h"
#include <Vector4.h>

enum class RenderTextureState {
	RenderTarget,
	PixelShaderResource
};

class DirectXBase
{
public:
	//シングルトンインスタンス
	static DirectXBase* GetInstance();
	//初期化
	void Initialize(WindowsAPI* windowsAPI);
	//終了
	void Finalize();
	//描画前処理
	void PreDraw();
	//描画後処理
	void PostDraw();
	//RenderTexture描画前処理
	void PreDrawRenderTexture();
	//swapchainに描画
	void DrawRenderTextureToScreen();
	//RenderTextureをSRV用に切り替え
	void TransitionRenderTextureToSRV();
	//ルートシグネチャ作成
	void CreateRootSignature();
	//グラフィックスパイプライン作成
	void CreatePipelineState();
	//テクスチャデータの転送
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
	//デスクリプタヒープを生成
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisivle);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, int32_t width, int32_t height);
	//シェーダーのコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob>CompileShader(const std::wstring& filePath, const wchar_t* profile);
	//バッファリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateBufferResource(size_t sizeInBytes);
	//テクスチャリソースの生成
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateTextureResource( const DirectX::TexMetadata& metadata);
	//レンダーテクスチャの生成
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device>device,
		uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);
	//テクスチャファイル読み込み
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);
	//最大SRV数(最大テクスチャ数)
	//static const uint32_t kMaxSRVCount;

public:
	//デバイスの取得
	Microsoft::WRL::ComPtr<ID3D12Device>Getdevice() { return device; }
	//コマンドリストの取得
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>Getcommandlist(){ return commandList; }
	//リソース取得
	ID3D12Resource* GetRenderTextureResource()const { return renderTextureResource.Get(); }
	//スワップチェーンリソースの数を取得
	size_t GetSwapChainResourcesNum()const { return swapChainDesc.BufferCount; }
	//CPUデスクリプタハンドル取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//GPUデスクリプタハンドル取得関数
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//DSV　CPUデスクリプタハンドル取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	//DSV　GPUデスクリプタハンドル取得関数
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

	//CPUデスクリプタハンドル取得関数
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//GPUデスクリプタハンドル取得関数
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	//RTVのデスクリプタハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);

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
	//FPS固定初期化
	void InitializeFixFPS();
	//FPS固定更新
	void UpdateFixFPS();
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
	//DSV用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvDescriptorHeap;
	//SRV用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>srvDescriptorHeap;
	//レンダーテクスチャ用のDescriptorSize
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>renderrtvDescriptorHeap;
	///深度ステンシルリソース
	Microsoft::WRL::ComPtr <ID3D12Resource> depthStencilResource;
	//フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	//DXCユーティリティ
	Microsoft::WRL::ComPtr<IDxcUtils>dxcUtils;
	//DXCコンパイラ 
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler;
	//デフォルトインクルードハンドラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler>includeHandler;
	//頂点リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	// ビューポート
	D3D12_VIEWPORT viewport{};
	// シザー矩形
	D3D12_RECT scissorRect{};
	//WindowsAPI
	WindowsAPI* windowsAPI = nullptr;
	//スワップチェインリソース
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swapChainResources;
	//レンダーテクスチャリソース
	Microsoft::WRL::ComPtr<ID3D12Resource>renderTextureResource;
	//レンダーテクスチャのSRV
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	//最大SRV数
	const uint32_t kMaxSRVCount = 512;
	//SRVデスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
	HANDLE fenceEvent;
	//フェンス値
	UINT64 fenceValue = 0;
	//スワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//RTVハンドルの要素数
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[3];
	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier{};
	HRESULT hr;
	//記録時間(FPS固定用)
	std::chrono::steady_clock::time_point reference_;

	RenderTextureState renderTextureState = RenderTextureState::RenderTarget;

	static DirectXBase* instance;
	DirectXBase* directxBase_ = nullptr;
};

