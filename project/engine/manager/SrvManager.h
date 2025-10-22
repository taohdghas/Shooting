#pragma once
#include "DirectXBase.h"

//SRV管理
class SrvManager
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// - 初回呼び出し時に内部でインスタンスを生成して返します。
	/// </summary>
	static SrvManager* GetInstance();
	/// <summary>
	/// 初期化を行う。
	/// - 指定した <c>DirectXBase</c> を保持し、SRV 用デスクリプタヒープを生成してデスクリプタサイズを取得します。
	/// </summary>
	/// <param name="directxBase">DirectX の共通処理を提供する <c>DirectXBase*</c>。</param>
	void Initialize(DirectXBase* directxBase);
	/// <summary>
	/// 終了処理を行う。
	/// - シングルトンインスタンスを破棄します（内部で delete を呼ぶ実装）。
	/// </summary>
	void Finalize();
	/// <summary>
	/// 2D テクスチャ用の SRV を作成する。
	/// - 指定したリソースとメタデータに基づき、CPU デスクリプタハンドル位置に SRV を作成します。
	/// </summary>
	/// <param name="srvIndex">SRV を格納するインデックス。</param>
	/// <param name="pResource">SRV を参照する D3D リソース。</param>
	/// <param name="metadata">テクスチャのメタデータ（フォーマット / ミップ数 / キューブ判定など）。</param>
	void CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DirectX::TexMetadata metadata);
	/// <summary>
	/// 構造化バッファ用（Structured Buffer） SRV を作成する。
	/// - バッファの要素数と要素バイト幅を指定して SRV を生成します。
	/// </summary>
	/// <param name="srvIndex">SRV を格納するインデックス。</param>
	/// <param name="pResource">SRV を参照する D3D リソース。</param>
	/// <param name="numElements">要素数。</param>
	/// <param name="structureByteStride">構造体のバイト幅。</param>
	void CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	/// <summary>
	/// レンダーテクスチャ用の SRV を作成する。
	/// - 内部のレンダーテクスチャリソースを参照して SRV を生成します（RenderTexture を SRV として扱うケース）。
	/// </summary>
	/// <param name="srvIndex">SRV を格納するインデックス。</param>
	/// <param name="pResource">対象リソース（未使用実装では内部 RenderTexture を使用）。</param>
	/// <param name="format">ピクセルフォーマット（使用時に参照）。</param>
	void CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format);
	/// <summary>
	/// 描画前に SRV 用のデスクリプタヒープをコマンドリストへセットする。
	/// - 描画ループ開始前に呼び出してデスクリプタヒープを有効化します。
	/// </summary>
	void PreDraw();
	/// <summary>
	/// ルートパラメータに対して指定 SRV インデックスの GPU デスクリプタハンドルをセットする。
	/// - 実際には内部で計算した GPU ハンドルを <c>ID3D12GraphicsCommandList::SetGraphicsRootDescriptorTable</c> に渡します。
	/// </summary>
	/// <param name="RootParameterIndex">ルートパラメータのインデックス。</param>
	/// <param name="srvIndex">セットする SRV のインデックス。</param>
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex);
	/// <summary>
	/// 新しい SRV を割り当て可能かチェックする。
	/// - 現在の使用インデックスが上限未満かで判定します。
	/// </summary>
	/// <returns>割り当て可能なら true、上限に達していれば false。</returns>
	bool Securedcheck();
	/// <summary>
	/// SRV インデックスを割り当てる。
	/// - 上限に達していないことをアサートし、現在のインデックスを返してインクリメントします。
	/// </summary>
	/// <returns>割り当てられた SRV インデックス。</returns>
	uint32_t Allcate();
	/// <summary>
	/// 指定インデックスに対応する CPU デスクリプタハンドルを計算して返す。
	/// - ヒープの先頭ハンドルに descripterSize * index を加算して算出します。
	/// </summary>
	/// <param name="index">計算対象のインデックス。</param>
	/// <returns>対応する D3D12_CPU_DESCRIPTOR_HANDLE。</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	/// <summary>
	/// 指定インデックスに対応する GPU デスクリプタハンドルを計算して返す。
	/// - ヒープの先頭 GPU ハンドルに descripterSize * index を加算して算出します。
	/// </summary>
	/// <param name="index">計算対象のインデックス。</param>
	/// <returns>対応する D3D12_GPU_DESCRIPTOR_HANDLE。</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
	/// <summary>
	/// 内部が保持するデスクリプタヒープの生ポインタを返す。
	/// - ImGui などがヒープのポインタを直接必要とする場面で利用されます。
	/// </summary>
	/// <returns>ID3D12DescriptorHeap*（ComPtr の生ポインタ）</returns>
	ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptorHeap.Get(); }
public:
	//最大SRV数(最大テクスチャ数)
	static const uint32_t kMaxCount;
private:
	static SrvManager* instance;
	SrvManager* srvManager_ = nullptr;

	DirectXBase* directxBase = nullptr;
	//SRV用DescriptorSizeを取得
	uint32_t descriptorSize;
	//次に使用するSRVindex
	uint32_t useIndex = 0;
	//SRV用デスクリプターヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;
};

