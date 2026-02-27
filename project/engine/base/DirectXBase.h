#pragma once
#include "Windows.h"
#include "WindowsAPI.h"
#include "PipeLineStateObject.h"
#include "Struct.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <array>
#include <dxcapi.h>
#include <string>
#include <chrono>
#include "externals/DirectXTex/DirectXTex.h"
#include <memory>

namespace MyEngine {

    // レンダーテクスチャ状態
    enum class RenderTextureState {
        RenderTarget,
        PixelShaderResource
    };

    // DirectX基盤クラス
    /// <summary>
    /// DirectX 12デバイスやコマンドリスト等のグラフィックス基盤リソースの生成・管理
    ///描画処理の前後処理やレンダーテクスチャ制御
    ///テクスチャやバッファ等のGPUリソース生成・管理
    ///シェーダーのコンパイルやデスクリプタヒープの生成
    ///スワップチェインやレンダーテクスチャの管理
    ///テクスチャファイルのロードユーティリティの提供
    ///シングルトンによるインスタンス管理
    /// </summary>
    class DirectXBase {
    public:
        /// <summary>
        /// シングルトンインスタンス
        /// <returns>DirectXBaseのインスタンス（ポインタ）</returns>
        /// </summary>
        static DirectXBase* GetInstance();

        /// <summary>
        /// 初期化
        /// <param name="windows_api">WindowsAPIクラスのポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void Initialize(WindowsApi* windows_api);

        /// <summary>
        /// 終了処理
        /// <returns>なし</returns>
        /// </summary>
        void Finalize();

        /// <summary>
        /// 描画開始前の共通処理
        /// <returns>なし</returns>
        /// </summary>
        void PreDraw();

        /// <summary>
        /// 描画終了後の共通処理
        /// <returns>なし</returns>
        /// </summary>
        void PostDraw();

        /// <summary>
        /// レンダーテクスチャへ描画する際の前処理
        /// <returns>なし</returns>
        /// </summary>
        void PreDrawRenderTexture();

        /// <summary>
        /// レンダーテクスチャの内容をスワップチェインに描画する処理
        /// <returns>なし</returns>
        /// </summary>
        void DrawRenderTextureToScreen();

        /// <summary>
        /// レンダーテクスチャのリソースを SRVへ遷移
        /// <returns>なし</returns>
        /// </summary>
        void TransitionRenderTextureToSRV();

        /// <summary>
        /// テクスチャデータを GPU に転送するユーティリティ
        /// <param name="texture">転送先のテクスチャリソース</param>
        /// <param name="mip_images">転送するScratchImageデータ</param>
        /// <returns>転送後のテクスチャリソース</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(
            Microsoft::WRL::ComPtr<ID3D12Resource> texture,
            const DirectX::ScratchImage& mip_images);

        /// <summary>
        /// デスクリプタヒープを生成
        /// <param name="heap_type">ヒープタイプ</param>
        /// <param name="num_descriptors">デスクリプタ数</param>
        /// <param name="shader_visible">シェーダーから参照可能か</param>
        /// <returns>ID3D12DescriptorHeapのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
            D3D12_DESCRIPTOR_HEAP_TYPE heap_type,
            UINT num_descriptors,
            bool shader_visible);

        /// <summary>
        /// 深度ステンシル用テクスチャリソースを生成
        /// <param name="device">デバイス</param>
        /// <param name="width">幅</param>
        /// <param name="height">高さ</param>
        /// <returns>ID3D12ResourceのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(
            Microsoft::WRL::ComPtr<ID3D12Device> device,
            int32_t width, int32_t height);

        /// <summary>
        /// シェーダーファイルをコンパイル
        /// <param name="file_path">シェーダーファイルのパス</param>
        /// <param name="profile">シェーダープロファイル</param>
        /// <returns>IDxcBlobのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
            const std::wstring& file_path, const wchar_t* profile);

        /// <summary>
        /// 汎用バッファリソースを生成
        /// <param name="size_in_bytes">バッファサイズ（バイト）</param>
        /// <returns>ID3D12ResourceのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t size_in_bytes);

        /// <summary>
        /// テクスチャリソースを生成
        /// <param name="metadata">テクスチャメタデータ</param>
        /// <returns>ID3D12ResourceのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(
            const DirectX::TexMetadata& metadata);

        /// <summary>
        /// レンダーテクスチャ用のリソースを生成
        /// <param name="device">デバイス</param>
        /// <param name="width">幅</param>
        /// <param name="height">高さ</param>
        /// <param name="format">フォーマット</param>
        /// <param name="clear_color">クリアカラー</param>
        /// <returns>ID3D12ResourceのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(
            Microsoft::WRL::ComPtr<ID3D12Device> device,
            uint32_t width, uint32_t height,
            DXGI_FORMAT format,
            const Vector4& clear_color);

        /// <summary>
        /// テクスチャファイルを読み込んで ScratchImage として返すユーティリティ
        /// <param name="file_path">テクスチャファイルのパス</param>
        /// <returns>DirectX::ScratchImage</returns>
        /// </summary>
        static DirectX::ScratchImage LoadTexture(const std::string& file_path);

        /// <summary>
        /// デバイスを取得
        /// <returns>ID3D12DeviceのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }

        /// <summary>
        /// コマンドリストを取得
        /// <returns>ID3D12GraphicsCommandListのComPtr</returns>
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return command_list_; }

        /// <summary>
        /// レンダーテクスチャのリソースポインタを取得
        /// <returns>ID3D12Resourceのポインタ</returns>
        /// </summary>
        ID3D12Resource* GetRenderTextureResource() const { return render_texture_resource_.Get(); }

        /// <summary>
        /// スワップチェインのバッファ数を取得
        /// <returns>バッファ数（size_t）</returns>
        /// </summary>
        size_t GetSwapChainResourcesNum() const { return swap_chain_desc_.BufferCount; }

        /// <summary>
        /// 指定デスクリプタヒープの CPU デスクリプタハンドルを取得
        /// <param name="descriptor_heap">デスクリプタヒープ</param>
        /// <param name="descriptor_size">デスクリプタサイズ</param>
        /// <param name="index">インデックス</param>
        /// <returns>D3D12_CPU_DESCRIPTOR_HANDLE</returns>
        /// </summary>
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap,
            uint32_t descriptor_size, uint32_t index);

        /// <summary>
        /// 指定デスクリプタヒープの GPU デスクリプタハンドルを取得
        /// <param name="descriptor_heap">デスクリプタヒープ</param>
        /// <param name="descriptor_size">デスクリプタサイズ</param>
        /// <param name="index">インデックス</param>
        /// <returns>D3D12_GPU_DESCRIPTOR_HANDLE</returns>
        /// </summary>
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap,
            uint32_t descriptor_size, uint32_t index);

        /// <summary>
        /// DSV 用 CPU デスクリプタハンドルを取得
        /// <param name="index">インデックス</param>
        /// <returns>D3D12_CPU_DESCRIPTOR_HANDLE</returns>
        /// </summary>
        D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

        /// <summary>
        /// DSV 用 GPU デスクリプタハンドルを取得
        /// <param name="index">インデックス</param>
        /// <returns>D3D12_GPU_DESCRIPTOR_HANDLE</returns>
        /// </summary>
        D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

    public:
        DirectXBase() = default;
        ~DirectXBase() = default;
    private:

        /// <summary>
        /// デバイスの初期化
        /// <returns>なし</returns>
        /// </summary>
        void DeviceInitialize();

        /// <summary>
        /// コマンド関連の初期化
        /// <returns>なし</returns>
        /// </summary>
        void CommandInitialize();

        /// <summary>
        /// スワップチェーンの生成
        /// <returns>なし</returns>
        /// </summary>
        void SwapchainGenerate();

        /// <summary>
        /// 深度バッファの生成
        /// <returns>なし</returns>
        /// </summary>
        void DepthbufferGenerate();

        /// <summary>
        /// デスクリプタヒープの生成
        /// <returns>なし</returns>
        /// </summary>
        void DescriptorheapGenerate();

        /// <summary>
        /// レンダーターゲットビュー(RTV)の初期化
        /// <returns>なし</returns>
        /// </summary>
        void RendertargetviewInitialize();

        /// <summary>
        /// 深度ステンシルビュー(DSV)の初期化
        /// <returns>なし</returns>
        /// </summary>
        void DepthstencilviewInitialize();

        /// <summary>
        /// フェンスの初期化
        /// <returns>なし</returns>
        /// </summary>
        void FenceInitialize();

        /// <summary>
        /// ビューポートの初期化
        /// <returns>なし</returns>
        /// </summary>
        void ViewportInitialize();

        /// <summary>
        /// シザー矩形の初期化
        /// <returns>なし</returns>
        /// </summary>
        void ScissorrectInitialize();

        /// <summary>
        /// DXCコンパイラの生成
        /// <returns>なし</returns>
        /// </summary>
        void DXCompilerInitialize();

        /// <summary>
        /// FPS固定用の初期化
        /// <returns>なし</returns>
        /// </summary>
        void InitializeFixFPS();

        /// <summary>
        /// FPS固定用の更新処理
        /// <returns>なし</returns>
        /// </summary>
        void UpdateFixFPS();

    private:
        // DirectX12デバイス
        Microsoft::WRL::ComPtr<ID3D12Device> device_;
        // DXGIファクトリ
        Microsoft::WRL::ComPtr<IDXGIFactory7> dxgi_factory_;
        // コマンドアロケータ
        Microsoft::WRL::ComPtr<ID3D12CommandAllocator> command_allocator_;
        // コマンドリスト
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> command_list_;
        // コマンドキュー
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> command_queue_;
        // スワップチェーン
        Microsoft::WRL::ComPtr<IDXGISwapChain4> swap_chain_;
        // Resource
        Microsoft::WRL::ComPtr<ID3D12Resource> resource_;

        // Descriptor sizes
        uint32_t descriptor_size_srv_ = 0;
        uint32_t descriptor_size_rtv_ = 0;
        uint32_t descriptor_size_dsv_ = 0;

        // Descriptor heaps
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtv_descriptor_heap_;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsv_descriptor_heap_;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srv_descriptor_heap_;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> render_rtv_descriptor_heap_;

        // 深度ステンシルリソース
        Microsoft::WRL::ComPtr<ID3D12Resource> depth_stencil_resource_;

        // フェンス
        Microsoft::WRL::ComPtr<ID3D12Fence> fence_;

        // DXCユーティリティ系
        Microsoft::WRL::ComPtr<IDxcUtils> dxc_utils_;
        Microsoft::WRL::ComPtr<IDxcCompiler3> dxc_compiler_;
        Microsoft::WRL::ComPtr<IDxcIncludeHandler> include_handler_;

        // 頂点リソース
        Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;

        // ビューポート
        D3D12_VIEWPORT viewport_{};

        // シザー矩形
        D3D12_RECT scissor_rect_{};

        // WindowsAPI
        WindowsApi* windows_api_ = nullptr;

        // PSO
        std::unique_ptr<PipelineStateObject> pso_;

        // スワップチェインリソース
        std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, 2> swap_chain_resources_;

        // レンダーテクスチャ
        Microsoft::WRL::ComPtr<ID3D12Resource> render_texture_resource_;

        // レンダーテクスチャ SRV 設定
        D3D12_SHADER_RESOURCE_VIEW_DESC render_texture_srv_desc_{};

        // 最大SRV数
        const uint32_t kMaxSrvCount = 512;

        // SRVデスクリプタヒープ
        D3D12_DESCRIPTOR_HEAP_DESC srv_heap_desc_{};

        // フェンスイベント
        HANDLE fence_event_;

        // フェンス値
        UINT64 fence_value_ = 0;

        // スワップチェイン設定
        DXGI_SWAP_CHAIN_DESC1 swap_chain_desc_{};

        // RTV設定
        D3D12_RENDER_TARGET_VIEW_DESC rtv_desc_{};

        // RTVハンドル
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_handles_[3];

        // Transition Barrier
        D3D12_RESOURCE_BARRIER barrier_{};

        // HRESULT
        HRESULT hr_;

        // 記録時間(FPS固定用)
        std::chrono::steady_clock::time_point reference_;

        // レンダーテクスチャ状態
        RenderTextureState render_texture_state_ = RenderTextureState::RenderTarget;

        // シングルトンインスタンス
        static std::unique_ptr<DirectXBase> instance_;
        // 自身を指すポインタ（用途不明だが名前規則だけ合わせる）
        DirectXBase* directx_base_ = nullptr;

        // コピーコンストラクタ禁止
        DirectXBase(const DirectXBase&) = delete;

        // 代入演算子禁止
        DirectXBase& operator=(const DirectXBase&) = delete;
    };
}