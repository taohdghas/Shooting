#pragma once
#include <d3d12.h>
#include <wrl.h>

namespace MyEngine {

    // DirectX基盤クラス前方宣言
    class DirectXBase;

    // パイプラインステートを管理するクラス
    /// <summary>
    ///DirectX 12の各種描画用途に対応した
    /// ルートシグネチャおよびグラフィックスパイプラインステートの生成・管理
    /// ルートシグネチャやパイプラインステートの共通生成処理の提供
    /// CBVやディスクリプタテーブル、静的サンプラー等のルートパラメータ設定補助
    /// 用途ごとのルートシグネチャ／パイプラインステートの取得インターフェースの提供
    /// DirectXBaseとの連携
    /// </summary>
    class PipelineStateObject {
    public:
        /// <summary>
        /// 初期化
        /// <param name="directx_base">DirectX基盤クラスのポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void Initialize(DirectXBase* directx_base);

        /// <summary>
        /// Object3D 用のルートシグネチャを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateRootSignature();

        /// <summary>
        /// Object3D 用のグラフィックスパイプラインステートを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreatePipelineState();

        /// <summary>
        /// Sprite 用のルートシグネチャを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateSpriteRootSignature();

        /// <summary>
        /// Sprite 用のグラフィックスパイプラインステートを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateSpritePipelineState();

        /// <summary>
        /// Particle 用のルートシグネチャを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateParticleRootSignature();

        /// <summary>
        /// Particle 用のグラフィックスパイプラインステートを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateParticlePipelineState();

        /// <summary>
        /// PostEffect 用のルートシグネチャを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreatePostEffectRootSignature();

        /// <summary>
        /// PostEffect 用のグラフィックスパイプラインステートを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreatePostEffectPipelineState();

        /// <summary>
        /// SkyBox 用のルートシグネチャを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateSkyBoxRootSignature();

        /// <summary>
        /// SkyBox 用のグラフィックスパイプラインステートを作成
        /// <returns>なし</returns>
        /// </summary>
        void CreateSkyBoxPipelineState();

    private:
        /// <summary>
        /// 共通のルートシグネチャを作成
        /// <param name="rootParameters">ルートパラメータ配列</param>
        /// <param name="numRootParams">ルートパラメータ数</param>
        /// <param name="descriptorRanges">ディスクリプタレンジ配列</param>
        /// <param name="numDescriptorRanges">ディスクリプタレンジ数</param>
        /// <param name="staticSamplers">静的サンプラー配列</param>
        /// <param name="numStaticSamplers">静的サンプラー数</param>
        /// <param name="outRootSignature">生成されたルートシグネチャ</param>
        /// <returns>なし</returns>
        /// </summary>
        void CreateRootSignatureCommon(
            D3D12_ROOT_PARAMETER* rootParameters, UINT numRootParams,
            D3D12_DESCRIPTOR_RANGE* descriptorRanges, UINT numDescriptorRanges,
            D3D12_STATIC_SAMPLER_DESC* staticSamplers, UINT numStaticSamplers,
            Microsoft::WRL::ComPtr<ID3D12RootSignature>& outRootSignature);

        /// <summary>
        /// 共通のグラフィックスパイプラインステートを作成
        /// <param name="rootSignature">ルートシグネチャ</param>
        /// <param name="inputLayoutDesc">入力レイアウト記述子</param>
        /// <param name="blendDesc">ブレンド記述子</param>
        /// <param name="rasterizerDesc">ラスタライザ記述子</param>
        /// <param name="depthStencilDesc">深度ステンシル記述子</param>
        /// <param name="rtvFormat">RTVフォーマット</param>
        /// <param name="dsvFormat">DSVフォーマット</param>
        /// <param name="vsPath">頂点シェーダファイルパス</param>
        /// <param name="vsProfile">頂点シェーダプロファイル</param>
        /// <param name="psPath">ピクセルシェーダファイルパス</param>
        /// <param name="psProfile">ピクセルシェーダプロファイル</param>
        /// <param name="outPipelineState">生成されたパイプラインステート</param>
        /// <returns>なし</returns>
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
        /// CBV用のルートパラメータを設定
        /// <param name="param">設定対象のルートパラメータ</param>
        /// <param name="shaderRegister">シェーダーレジスタ番号</param>
        /// <param name="visibility">シェーダー可視性</param>
        /// <returns>なし</returns>
        /// </summary>
        void SetRootParameterCBV(D3D12_ROOT_PARAMETER& param, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

        /// <summary>
        /// ディスクリプタテーブル用のルートパラメータを設定
        /// <param name="param">設定対象のルートパラメータ</param>
        /// <param name="range">ディスクリプタレンジ配列</param>
        /// <param name="numRanges">ディスクリプタレンジ数</param>
        /// <param name="visibility">シェーダー可視性</param>
        /// <returns>なし</returns>
        /// </summary>
        void SetRootParameterDescriptorTable(D3D12_ROOT_PARAMETER& param, D3D12_DESCRIPTOR_RANGE* range, UINT numRanges, D3D12_SHADER_VISIBILITY visibility);

        /// <summary>
        /// 静的サンプラーの設定
        /// <param name="sampler">設定対象の静的サンプラー</param>
        /// <param name="filter">フィルタ</param>
        /// <param name="addressU">アドレスモードU</param>
        /// <param name="addressV">アドレスモードV</param>
        /// <param name="addressW">アドレスモードW</param>
        /// <param name="shaderRegister">シェーダーレジスタ番号</param>
        /// <param name="visibility">シェーダー可視性</param>
        /// <returns>なし</returns>
        /// </summary>
        void SetStaticSampler(D3D12_STATIC_SAMPLER_DESC& sampler, D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV, D3D12_TEXTURE_ADDRESS_MODE addressW, UINT shaderRegister, D3D12_SHADER_VISIBILITY visibility);

    public:
        /// <summary>Object3D 用のルートシグネチャを取得</summary>
        ID3D12RootSignature* GetRootSignature() const { return root_signature_.Get(); }
        /// <summary>Object3D 用のグラフィックスパイプラインステートを取得</summary>
        ID3D12PipelineState* GetGraphicsPipelineState() const { return graphics_pipeline_state_.Get(); }

        /// <summary>Sprite 用のルートシグネチャを取得</summary>
        ID3D12RootSignature* GetSpriteRootSignature() const { return sprite_root_signature_.Get(); }
        /// <summary>Sprite 用のグラフィックスパイプラインステートを取得。</summary>
        ID3D12PipelineState* GetSpriteGraphicsPipelineState() const { return sprite_graphics_pipeline_state_.Get(); }

        /// <summary>Particle 用のルートシグネチャを取得</summary>
        ID3D12RootSignature* GetParticleRootSignature() const { return particle_root_signature_.Get(); }
        /// <summary>Particle 用のグラフィックスパイプラインステートを取得</summary>
        ID3D12PipelineState* GetParticleGraphicsPipelineState() const { return particle_graphics_pipeline_state_.Get(); }

        /// <summary>PostEffect 用のルートシグネチャを取得</summary>
        ID3D12RootSignature* GetPostEffectRootSignature() const { return post_effect_root_signature_.Get(); }
        /// <summary>PostEffect 用のグラフィックスパイプラインステートを取得</summary>
        ID3D12PipelineState* GetPostEffectGraphicsPipelineState() const { return post_effect_graphics_pipeline_state_.Get(); }

        /// <summary>SkyBox 用のルートシグネチャを取得</summary>
        ID3D12RootSignature* GetSkyBoxRootSignature() const { return sky_box_root_signature_.Get(); }
        /// <summary>SkyBox 用のグラフィックスパイプラインステートを取得</summary>
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