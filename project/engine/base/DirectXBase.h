#pragma once
#include "Windows.h"
#include "WindowsAPI.h"
#include "Pso.h"
#include "Struct.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <dxcapi.h>
#include <string>
#include <chrono>
#include "externals/DirectXTex/DirectXTex.h"

//レンダーテクスチャ状態
enum class RenderTextureState {
	RenderTarget,
	PixelShaderResource
};
//DirectX基盤クラス
class DirectXBase
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// 必要に応じて内部で単一インスタンスを生成して返す想定。
	/// </summary>
	static DirectXBase* GetInstance();
	/// <summary>
	/// 初期化を行う。
	/// WindowsAPI の情報を受け取り、DirectX デバイスやスワップチェイン等の初期化を行う。
	/// </summary>
	/// <param name="windowsAPI">ウィンドウ関連情報を提供する WindowsAPI のポインタ。</param>
	void Initialize(WindowsAPI* windowsAPI);
	/// <summary>
	/// 終了処理を行う。
	/// 確保したリソースやデバイス、デスクリプタヒープなどを解放する。
	/// </summary>
	void Finalize();
	/// <summary>
	/// 描画開始前の共通処理を行う。
	/// - コマンドリストの準備、レンダーターゲットの遷移などを行う。
	/// </summary>
	void PreDraw();
	/// <summary>
	/// 描画終了後の共通処理を行う。
	/// - コマンドリストのクローズ、キューへの送信、フレーム同期などを行う。
	/// </summary>
	void PostDraw();
	/// <summary>
	/// レンダーテクスチャへ描画する際の前処理を行う。
	/// - レンダーターゲットの切り替えやクリアなどを行う。
	/// </summary>
	void PreDrawRenderTexture();
	/// <summary>
	/// レンダーテクスチャの内容をスワップチェインに描画する処理を行う。
	/// - フルスクリーン合成やシェーダリソースからの描画を想定。
	/// </summary>
	void DrawRenderTextureToScreen();
	/// <summary>
	/// レンダーテクスチャのリソースを SRV（ピクセルシェーダで参照可能）へ遷移する。
	/// </summary>
	void TransitionRenderTextureToSRV();
	/// <summary>
	/// テクスチャデータを GPU に転送するユーティリティ。
	/// - 提供された ScratchImage のミップデータを指定テクスチャリソースへアップロードする。
	/// </summary>
	/// <param name="texture">アップロード先のテクスチャリソース（ComPtr）。</param>
	/// <param name="mipImages">アップロードするイメージデータ。</param>
	/// <returns>アップロード済みのテクスチャリソース（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const DirectX::ScratchImage& mipImages);
	/// <summary>
	/// デスクリプタヒープを生成する。
	/// </summary>
	/// <param name="heapType">ヒープの種類（SRV/RTV/DSV/etc）。</param>
	/// <param name="numDescriptors">要素数。</param>
	/// <param name="shaderVisivle">シェーダから見えるヒープにするか。</param>
	/// <returns>生成したデスクリプタヒープ（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisivle);
	/// <summary>
	/// 深度ステンシル用テクスチャリソースを生成する。
	/// </summary>
	/// <param name="device">使用する D3D12Device。</param>
	/// <param name="width">幅。</param>
	/// <param name="height">高さ。</param>
	/// <returns>生成した深度ステンシルリソース（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(Microsoft::WRL::ComPtr<ID3D12Device> device, int32_t width, int32_t height);
	/// <summary>
	/// シェーダーファイルをコンパイルする。
	/// </summary>
	/// <param name="filePath">コンパイルするシェーダファイルのパス（ワイド文字列）。</param>
	/// <param name="profile">ターゲットプロファイル（例: L&quot;ps_6_0&quot;）。</param>
	/// <returns>コンパイル済みバイナリを保持する IDxcBlob（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<IDxcBlob>CompileShader(const std::wstring& filePath, const wchar_t* profile);
	/// <summary>
	/// 汎用バッファリソースを生成する。
	/// </summary>
	/// <param name="sizeInBytes">バッファサイズ（バイト）。</param>
	/// <returns>生成したバッファリソース（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateBufferResource(size_t sizeInBytes);
	/// <summary>
	/// テクスチャリソースを生成する。
	/// </summary>
	/// <param name="metadata">テクスチャのメタデータ（DirectXTex の TexMetadata）。</param>
	/// <returns>生成したテクスチャリソース（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateTextureResource( const DirectX::TexMetadata& metadata);
	/// <summary>
	/// レンダーテクスチャ用のリソースを生成する。
	/// </summary>
	/// <param name="device">使用する D3D12Device。</param>
	/// <param name="width">幅。</param>
	/// <param name="height">高さ。</param>
	/// <param name="format">ピクセルフォーマット。</param>
	/// <param name="clearColor">クリアカラー（必要に応じて格納）。</param>
	/// <returns>生成したレンダーテクスチャリソース（ComPtr）。</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource>CreateRenderTextureResource(Microsoft::WRL::ComPtr<ID3D12Device>device,
		uint32_t width, uint32_t height, DXGI_FORMAT format, const Vector4& clearColor);
	/// <summary>
	/// テクスチャファイルを読み込んで ScratchImage として返すユーティリティ。
	/// </summary>
	/// <param name="filePath">読み込むテクスチャファイルのパス。</param>
	/// <returns>読み込んだイメージデータを格納した DirectX::ScratchImage。</returns>
	static DirectX::ScratchImage LoadTexture(const std::string& filePath);

public:
	/// <summary>
	/// デバイスを取得する。
	/// </summary>
	/// <returns>ID3D12Device の ComPtr。</returns>
	Microsoft::WRL::ComPtr<ID3D12Device>Getdevice() { return device; }
	/// <summary>
	/// コマンドリストを取得する。
	/// </summary>
	/// <returns>ID3D12GraphicsCommandList の ComPtr。</returns>
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>Getcommandlist(){ return commandList; }
	/// <summary>
	/// レンダーテクスチャのリソースポインタを取得する。
	/// </summary>
	/// <returns>レンダーテクスチャの生ポインタ（nullptr 可）。</returns>
	ID3D12Resource* GetRenderTextureResource()const { return renderTextureResource.Get(); }
	/// <summary>
	/// スワップチェインのバッファ数を取得する。
	/// </summary>
	/// <returns>スワップチェインのバッファ数。</returns>
	size_t GetSwapChainResourcesNum()const { return swapChainDesc.BufferCount; }
	/// <summary>
	/// 指定デスクリプタヒープの CPU デスクリプタハンドルを取得する。
	/// </summary>
	/// <param name="descriptorHeap">対象のデスクリプタヒープ。</param>
	/// <param name="descriptorSize">デスクリプタのサイズ（デバイスから取得する値）。</param>
	/// <param name="index">インデックス。</param>
	/// <returns>指定要素の CPU デスクリプタハンドル。</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	/// <summary>
	/// 指定デスクリプタヒープの GPU デスクリプタハンドルを取得する。
	/// </summary>
	/// <param name="descriptorHeap">対象のデスクリプタヒープ。</param>
	/// <param name="descriptorSize">デスクリプタのサイズ（デバイスから取得する値）。</param>
	/// <param name="index">インデックス。</param>
	/// <returns>指定要素の GPU デスクリプタハンドル。</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>
		& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	/// <summary>
	/// DSV 用の CPU デスクリプタハンドルを取得する。
	/// </summary>
	/// <param name="index">インデックス。</param>
	/// <returns>DSV の CPU デスクリプタハンドル。</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
	/// <summary>
	/// DSV 用の GPU デスクリプタハンドルを取得する。
	/// </summary>
	/// <param name="index">インデックス。</param>
	/// <returns>DSV の GPU デスクリプタハンドル。</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

private:
	/// <summary>コンストラクタ（プライベート：シングルトン）。</summary>
	DirectXBase() = default;
	/// <summary>デストラクタ（プライベート）。</summary>
	~DirectXBase() = default;
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
	//Pso
	std::unique_ptr<Pso>pso_;
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
	//コピーコンストラクタを無効にする
	DirectXBase(const DirectXBase&) = delete;
	//代入演算子を無効にする
	DirectXBase& operator = (const DirectXBase&) = delete;
};

