#pragma once
#include "Windows.h"
#include "DirectXBase.h"
#include "SrvManager.h"
#include <string>
#include <unordered_map>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

// テクスチャ管理クラス
class TextureManager {
public:
    /// <summary>
    /// シングルトンインスタンスを取得する。
    /// - 初回呼び出し時に内部でインスタンスを生成して返す。
    /// </summary>
    static TextureManager* GetInstance();

    /// <summary>
    /// 終了処理を行う。
    /// - 内部で確保したシングルトンインスタンスを破棄する（Finalize は TextureManager::instance_ を delete する実装）。
    /// </summary>
    void Finalize();

    /// <summary>
    /// 初期化を行う。
    /// - DirectXBase と SrvManager のポインタを保持し、以降のテクスチャ読み込みや SRV 作成で利用する。
    /// </summary>
    /// <param name="directx_base">DirectX の共通処理を提供する <c>DirectXBase*</c>。</param>
    /// <param name="srv_manager">SRV 割り当ておよびデスクリプタ管理を行う <c>SrvManager*</c>。</param>
    void Initialize(DirectXBase* directx_base, SrvManager* srv_manager);

    /// <summary>
    /// テクスチャファイルを読み込む。
    /// - 指定ファイルが未ロードであればファイルを読み込み、必要なミップ生成／GPU 転送を行い SRV を作成して内部マップに登録する。
    /// - .dds とそれ以外（WIC）を自動で判別し、圧縮フォーマットはそのまま、非圧縮はミップマップを生成する実装を想定する。
    /// </summary>
    /// <param name="file_path">読み込むテクスチャのファイルパス（例: "resources/texture.png"）。</param>
    void LoadTexture(const std::string& file_path);

    /// <summary>
    /// ファイルパスから SRV インデックス（割り当てられた番号）を取得する。
    /// - 読み込み済みであればその SRV インデックスを返す。未ロードの場合は 0 を返す実装（呼び出し側でのチェック推奨）。
    /// </summary>
    /// <param name="file_path">テクスチャのファイルパス。</param>
    /// <returns>SRV インデックス（未登録なら 0）。</returns>
    uint32_t GetTextureIndexByFilePath(const std::string& file_path);

    /// <summary>
    /// 指定ファイルパスに対応するテクスチャのメタデータを取得する。
    /// - 呼び出し前に該当テクスチャがロード済みであること（assert でチェックする実装）。
    /// </summary>
    /// <param name="file_path">テクスチャのファイルパス。</param>
    /// <returns>該当テクスチャの <c>DirectX::TexMetadata</c> の const 参照。</returns>
    const DirectX::TexMetadata& GetMetaData(const std::string& file_path);

    /// <summary>
    /// 指定ファイルパスに対応する SRV インデックスを取得する。
    /// - 例: シェーダに渡すための SRV 番号を取得する用途。
    /// </summary>
    /// <param name="file_path">テクスチャのファイルパス。</param>
    /// <returns>対応する SRV インデックス。</returns>
    uint32_t GetSrvIndex(const std::string& file_path);

    /// <summary>
    /// 指定ファイルパスに対応する GPU 側のデスクリプタハンドルを取得する。
    /// - SRV をコマンドリストへバインドする際に使用する GPU ハンドルを返す。
    /// </summary>
    /// <param name="file_path">テクスチャのファイルパス。</param>
    /// <returns>対応する <c>D3D12_GPU_DESCRIPTOR_HANDLE</c>。</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& file_path);

private:
    // テクスチャ1枚分のデータ
    struct TextureData {
        std::string file_path_;
        DirectX::TexMetadata metadata_;
        Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
        Microsoft::WRL::ComPtr<ID3D12Resource> intermediate_resource_;
        uint32_t srv_index_;
        D3D12_CPU_DESCRIPTOR_HANDLE srv_handle_cpu_;
        D3D12_GPU_DESCRIPTOR_HANDLE srv_handle_gpu_;
    };

    DirectXBase* directx_base_ = nullptr;
    SrvManager* srv_manager_ = nullptr;
    static TextureManager* instance_;
    // SRVインデックスの開始番号
    static uint32_t k_srv_index_top_;

    TextureManager() = default;
    ~TextureManager() = default;
    TextureManager(TextureManager&) = delete;
    TextureManager& operator=(TextureManager&) = delete;

    // テクスチャデータ
    std::unordered_map<std::string, TextureData> texture_datas_;
};
