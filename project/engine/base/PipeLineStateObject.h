#pragma once
#include <d3d12.h>
#include <wrl.h>

namespace MyEngine {

    // DirectX基盤クラス前方宣言
    class DirectXBase;

    // パイプラインステートを管理するクラス
    class PipelineStateObject {
    public:
        /// <summary>
        /// 初期化する。
        /// </summary>
        void Initialize(DirectXBase* directx_base);

        /// <summary>
        /// Object3D 用のルートシグネチャを作成する。
        /// </summary>
        void CreateRootSignature();

        /// <summary>
        /// Object3D 用のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreatePipelineState();

        /// <summary>
        /// Sprite 用のルートシグネチャを作成する。
        /// </summary>
        void CreateSpriteRootSignature();

        /// <summary>
        /// Sprite 用のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreateSpritePipelineState();

        /// <summary>
        /// Particle 用のルートシグネチャを作成する。
        /// </summary>
        void CreateParticleRootSignature();

        /// <summary>
        /// Particle 用のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreateParticlePipelineState();

        /// <summary>
        /// PostEffect 用のルートシグネチャを作成する。
        /// </summary>
        void CreatePostEffectRootSignature();

        /// <summary>
        /// PostEffect 用のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreatePostEffectPipelineState();

        /// <summary>
        /// SkyBox 用のルートシグネチャを作成する。
        /// </summary>
        void CreateSkyBoxRootSignature();

        /// <summary>
        /// SkyBox 用のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreateSkyBoxPipelineState();

    private:
        /// <summary>
        /// 共通のルートシグネチャを作成する。
        /// </summary>
        void CreateRootSignatureCommon(
            D3D12_ROOT_PARAMETER* rootParameters, UINT numRootParams,
            D3D12_DESCRIPTOR_RANGE* descriptorRanges, UINT numDescriptorRanges,
            D3D12_STATIC_SAMPLER_DESC* staticSamplers, UINT numStaticSamplers,
            Microsoft::WRL::ComPtr<ID3D12RootSignature>& outRootSignature);

        /// <summary>
        /// 共通のグラフィックスパイプラインステートを作成する。
        /// </summary>
        void CreateGraphicsPipelineStateCommon(
            ID3D12RootSignature* rootSignature,
            const D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc,
            const D3D12_BLEND_DESC& blendDesc,
            const D3D12_RASTERIZER_DESC& rasterizerDesc,
            const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc,
            DXGI_FORMAT rtvFormat,
            DXGI_FORMAT dsvFormat,
            const wchar_t* vsPath,
            const wchar_t* vsProfile,
            const wchar_t* psPath,
            const wchar_t* psProfile,
            Microsoft::WRL::ComPtr<ID3D12PipelineState>& outPipelineState);

        /// <summary>
        /// CBV（定数バッファビュー）用のルートパラメータを設定する。
        /// </summary>
        void SetRootParameterCBV(D3D12_ROOT_PARAMETER& param, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

        /// <summary>
        /// ディスクリプタテーブル用のルートパラメータを設定する。
        /// </summary>
        void SetRootParameterDescriptorTable(D3D12_ROOT_PARAMETER& param, D3D12_DESCRIPTOR_RANGE* range, UINT numRanges, D3D12_SHADER_VISIBILITY visibility);

        /// <summary>
        /// 静的サンプラーの設定を行う。
        /// </summary>
        void SetStaticSampler(D3D12_STATIC_SAMPLER_DESC& sampler, D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV, D3D12_TEXTURE_ADDRESS_MODE addressW, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

    public:
        /// <summary>Object3D 用のルートシグネチャを取得する。</summary>
        ID3D12RootSignature* GetRootSignature() const { return root_signature_.Get(); }
        /// <summary>Object3D 用のグラフィックスパイプラインステートを取得する。</summary>
        ID3D12PipelineState* GetGraphicsPipelineState() const { return graphics_pipeline_state_.Get(); }

        /// <summary>Sprite 用のルートシグネチャを取得する。</summary>
        ID3D12RootSignature* GetSpriteRootSignature() const { return sprite_root_signature_.Get(); }
        /// <summary>Sprite 用のグラフィックスパイプラインステートを取得する。</summary>
        ID3D12PipelineState* GetSpriteGraphicsPipelineState() const { return sprite_graphics_pipeline_state_.Get(); }

        /// <summary>Particle 用のルートシグネチャを取得する。</summary>
        ID3D12RootSignature* GetParticleRootSignature() const { return particle_root_signature_.Get(); }
        /// <summary>Particle 用のグラフィックスパイプラインステートを取得する。</summary>
        ID3D12PipelineState* GetParticleGraphicsPipelineState() const { return particle_graphics_pipeline_state_.Get(); }

        /// <summary>PostEffect 用のルートシグネチャを取得する。</summary>
        ID3D12RootSignature* GetPostEffectRootSignature() const { return post_effect_root_signature_.Get(); }
        /// <summary>PostEffect 用のグラフィックスパイプラインステートを取得する。</summary>
        ID3D12PipelineState* GetPostEffectGraphicsPipelineState() const { return post_effect_graphics_pipeline_state_.Get(); }

        /// <summary>SkyBox 用のルートシグネチャを取得する。</summary>
        ID3D12RootSignature* GetSkyBoxRootSignature() const { return sky_box_root_signature_.Get(); }
        /// <summary>SkyBox 用のグラフィックスパイプラインステートを取得する。</summary>
        ID3D12PipelineState* GetSkyBoxGraphicsPipelineState() const { return sky_box_graphics_pipeline_state_.Get(); }

    private:
        DirectXBase* directx_base_ = nullptr;

        // Object3D用
        Microsoft::WRL::ComPtr<ID3D12RootSignature> root_signature_;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> graphics_pipeline_state_;

        // Sprite用
        Microsoft::WRL::ComPtr<ID3D12RootSignature> sprite_root_signature_;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> sprite_graphics_pipeline_state_;

        // Particle用
        Microsoft::WRL::ComPtr<ID3D12RootSignature> particle_root_signature_;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> particle_graphics_pipeline_state_;

        // PostEffect用
        Microsoft::WRL::ComPtr<ID3D12RootSignature> post_effect_root_signature_;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> post_effect_graphics_pipeline_state_;

        // SkyBox用
        Microsoft::WRL::ComPtr<ID3D12RootSignature> sky_box_root_signature_;
        Microsoft::WRL::ComPtr<ID3D12PipelineState> sky_box_graphics_pipeline_state_;
    };
}