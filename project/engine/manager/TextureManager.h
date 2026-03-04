#pragma once
#include "Windows.h"
#include "DirectXBase.h"
#include "SrvManager.h"
#include <string>
#include <unordered_map>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

namespace MyEngine {

	// テクスチャ管理クラス
    /// <summary>
    /// テクスチャリソースの一元的な読み込み・管理・解放
    /// ファイルパスによるテクスチャの登録・検索・取得インターフェースの提供
    /// DirectX 12リソースやSRVの生成・管理
    /// テクスチャメタデータやSRVインデックス、デスクリプタハンドルの管理
    /// DirectXBaseやSrvManagerとの連携
    /// シングルトンによるインスタンス管理
    /// </summary>
	class TextureManager {
	public:
		/// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>TextureManagerのインスタンス（ポインタ）</returns>
        /// </summary>
		static TextureManager* GetInstance();

		/// <summary>
		/// 終了処理
		/// <returns>なし</returns>
		/// </summary>
		void Finalize();

		/// <summary>
		/// 初期化
		/// <param name="directx_base">DirectXBaseクラスのポインタ</param>
		/// <param name="srv_manager">SrvManagerクラスのポインタ</param>
		/// <returns>なし</returns>
		/// </summary>
		void Initialize(DirectXBase* directx_base, SrvManager* srv_manager);

		/// <summary>
		/// テクスチャファイルを読み込む
		/// <param name="file_path">テクスチャファイルのパス（std::string）</param>
		/// <returns>なし</returns>
		/// </summary>
		void LoadTexture(const std::string& file_path);

		/// <summary>
		/// ファイルパスから SRV インデックスを取得
		/// <param name="file_path">テクスチャファイルのパス（std::string）</param>
		/// <returns>SRVインデックス（uint32_t）</returns>
		/// </summary>
		uint32_t GetTextureIndexByFilePath(const std::string& file_path);

		/// <summary>
		/// 指定ファイルパスに対応するテクスチャのメタデータを取得
		/// <param name="file_path">テクスチャファイルのパス（std::string）</param>
		/// <returns>テクスチャメタデータ（const DirectX::TexMetadata&）</returns>
		/// </summary>
		const DirectX::TexMetadata& GetMetaData(const std::string& file_path);

		/// <summary>
		/// 指定ファイルパスに対応する SRV インデックスを取得
		/// <param name="file_path">テクスチャファイルのパス（std::string）</param>
		/// <returns>SRVインデックス（uint32_t）</returns>
		/// </summary>
		uint32_t GetSrvIndex(const std::string& file_path);

		/// <summary>
		/// 指定ファイルパスに対応する GPU 側のデスクリプタハンドルを取得
		/// <param name="file_path">テクスチャファイルのパス（std::string）</param>
		/// <returns>GPUデスクリプタハンドル（D3D12_GPU_DESCRIPTOR_HANDLE）</returns>
		/// </summary>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& file_path);
	public:
		TextureManager() = default;
		~TextureManager() = default;
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
		static std::unique_ptr<TextureManager> instance_;
		// SRVインデックスの開始番号
		static uint32_t k_srv_index_top_;

		TextureManager(TextureManager&) = delete;
		TextureManager& operator=(TextureManager&) = delete;

		// テクスチャデータ
		std::unordered_map<std::string, TextureData> texture_datas_;
	};
}