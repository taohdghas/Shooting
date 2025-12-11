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
    class DirectXBase
    {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static DirectXBase* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// </summary>
        void Initialize(WindowsApi* windows_api);

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// 描画開始前の共通処理を行う。
        /// </summary>
        void PreDraw();

        /// <summary>
        /// 描画終了後の共通処理を行う。
        /// </summary>
        void PostDraw();

        /// <summary>
        /// レンダーテクスチャへ描画する際の前処理を行う。
        /// </summary>
        void PreDrawRenderTexture();

        /// <summary>
        /// レンダーテクスチャの内容をスワップチェインに描画する処理を行う。
        /// </summary>
        void DrawRenderTextureToScreen();

        /// <summary>
        /// レンダーテクスチャのリソースを SRV（ピクセルシェーダで参照可能）へ遷移する。
        /// </summary>
        void TransitionRenderTextureToSRV();

        /// <summary>
        /// テクスチャデータを GPU に転送するユーティリティ。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(
            Microsoft::WRL::ComPtr<ID3D12Resource> texture,
            const DirectX::ScratchImage& mip_images);

        /// <summary>
        /// デスクリプタヒープを生成する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
            D3D12_DESCRIPTOR_HEAP_TYPE heap_type,
            UINT num_descriptors,
            bool shader_visible);

        /// <summary>
        /// 深度ステンシル用テクスチャリソースを生成する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateDepthStencilTextureResource(
            Microsoft::WRL::ComPtr<ID3D12Device> device,
            int32_t width, int32_t height);

        /// <summary>
        /// シェーダーファイルをコンパイルする。
        /// </summary>
        Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(
            const std::wstring& file_path, const wchar_t* profile);

        /// <summary>
        /// 汎用バッファリソースを生成する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t size_in_bytes);

        /// <summary>
        /// テクスチャリソースを生成する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(
            const DirectX::TexMetadata& metadata);

        /// <summary>
        /// レンダーテクスチャ用のリソースを生成する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Resource> CreateRenderTextureResource(
            Microsoft::WRL::ComPtr<ID3D12Device> device,
            uint32_t width, uint32_t height,
            DXGI_FORMAT format,
            const Vector4& clear_color);

        /// <summary>
        /// テクスチャファイルを読み込んで ScratchImage として返すユーティリティ。
        /// </summary>
        static DirectX::ScratchImage LoadTexture(const std::string& file_path);

    public:
        /// <summary>
        /// デバイスを取得する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device_; }

        /// <summary>
        /// コマンドリストを取得する。
        /// </summary>
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return command_list_; }

        /// <summary>
        /// レンダーテクスチャのリソースポインタを取得する。
        /// </summary>
        ID3D12Resource* GetRenderTextureResource() const { return render_texture_resource_.Get(); }

        /// <summary>
        /// スワップチェインのバッファ数を取得する。
        /// </summary>
        size_t GetSwapChainResourcesNum() const { return swap_chain_desc_.BufferCount; }

        /// <summary>
        /// 指定デスクリプタヒープの CPU デスクリプタハンドルを取得する。
        /// </summary>
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap,
            uint32_t descriptor_size, uint32_t index);

        /// <summary>
        /// 指定デスクリプタヒープの GPU デスクリプタハンドルを取得する。
        /// </summary>
        D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(
            Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& descriptor_heap,
            uint32_t descriptor_size, uint32_t index);

        /// <summary>
        /// DSV 用 CPU デスクリプタハンドルを取得する。
        /// </summary>
        D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);

        /// <summary>
        /// DSV 用 GPU デスクリプタハンドルを取得する。
        /// </summary>
        D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);

    public:
        DirectXBase() = default;
        ~DirectXBase() = default;
    private:

        // デバイスの初期化
        void DeviceInitialize();
        // コマンド関連の初期化
        void CommandInitialize();
        // スワップチェーンの生成
        void SwapchainGenerate();
        // 深度バッファの生成
        void DepthbufferGenerate();
        // デスクリプタヒープの生成
        void DescriptorheapGenerate();
        // RTV の初期化
        void RendertargetviewInitialize();
        // DSV の初期化
        void DepthstencilviewInitialize();
        // フェンスの初期化
        void FenceInitialize();
        // ビューポートの初期化
        void ViewportInitialize();
        // シザー矩形の初期化
        void ScissorrectInitialize();
        // DXCコンパイラの生成
        void DXCcompilerInitialize();
        // FPS固定初期化
        void InitializeFixFPS();
        // FPS固定更新
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