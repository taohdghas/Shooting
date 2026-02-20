#pragma once
#include "DirectXBase.h"

namespace MyEngine {

    // SRV管理クラス
    /// <summary>
    /// DirectX 12のSRV用デスクリプタヒープの生成・管理
    /// 2Dテクスチャ、構造化バッファ、レンダーテクスチャ等のSRV生成処理の提供
    /// SRVインデックスの割り当て・管理とハンドル計算
    /// コマンドリストへのデスクリプタヒープセットやルートパラメータへのSRVバインド
    /// SRVリソースの最大数管理と利用可能チェック
    /// DirectXBaseとの連携
    /// シングルトンによるインスタンス管理
    /// </summary>
    class SrvManager {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static SrvManager* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// </summary>
        void Initialize(DirectXBase* directx_base);

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// 2D テクスチャ用の SRV を作成する。
        /// </summary>
        void CreateSrvForTexture2D(uint32_t srv_index, ID3D12Resource* p_resource, const DirectX::TexMetadata& metadata);

        /// <summary>
        /// 構造化バッファ用（Structured Buffer） SRV を作成する。
        /// </summary>
        void CreateSrvForStructuredBuffer(uint32_t srv_index, ID3D12Resource* p_resource, UINT num_elements, UINT structure_byte_stride);

        /// <summary>
        /// レンダーテクスチャ用の SRV を作成する
        /// </summary>
        void CreateSrvForRenderTexture(uint32_t srv_index, ID3D12Resource* p_resource, DXGI_FORMAT format);

        /// <summary>
        /// 描画前に SRV 用のデスクリプタヒープをコマンドリストへセットする。
        /// </summary>
        void PreDraw();

        /// <summary>
        /// ルートパラメータに対して指定 SRV インデックスの GPU デスクリプタハンドルをセットする。
        /// </summary>
        void SetGraphicsRootDescriptorTable(UINT root_parameter_index, uint32_t srv_index);

        /// <summary>
        /// 新しい SRV を割り当て可能かチェックする。
        /// </summary>
        bool SecuredCheck();

        /// <summary>
        /// SRV インデックスを割り当てる。
        /// </summary>
        uint32_t Allocate();

        /// <summary>
        /// 指定インデックスに対応する CPU デスクリプタハンドルを計算して返す。
        /// </summary>
        D3D12_CPU_DESCRIPTOR_HANDLE GetCpuDescriptorHandle(uint32_t index);

        /// <summary>
        /// 指定インデックスに対応する GPU デスクリプタハンドルを計算して返す。
        /// </summary>
        D3D12_GPU_DESCRIPTOR_HANDLE GetGpuDescriptorHandle(uint32_t index);

        /// <summary>
        /// 内部が保持するデスクリプタヒープの生ポインタを返す
        /// </summary>
        ID3D12DescriptorHeap* GetDescriptorHeap() { return descriptor_heap_.Get(); }

    public:
        // 最大SRV数(最大テクスチャ数)
        static const uint32_t kMaxCount;

    private:
        static std::unique_ptr<SrvManager> instance_;

        DirectXBase* directx_base_ = nullptr;
        // SRV用DescriptorSizeを取得
        uint32_t descriptor_size_ = 0;
        // 次に使用するSRV index
        uint32_t use_index_ = 0;
        // SRV用デスクリプタヒープ
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
    };
}