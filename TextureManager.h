#pragma once
#include "Windows.h"
#include "DirectXBase.h"
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"

class TextureManager
{
public:
	//シングルトンインスタンスの取得
	static TextureManager* GetInstance();
	//終了
	void Finalize();
	//初期化
	void Initialize();
	//テクスチャファイルの読み込み
	void LoadTexture(const std::string& filePath);
private:
	//テクスチャ1枚分のデータ
	struct TextureData {
		std::string filePath;
		DirectX::TexMetadata metadata;
		Microsoft::WRL::ComPtr<ID3D12Resource>resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	DirectXBase* directxBase_;
	static TextureManager* instance;
	//SRVインデックスの開始番号
	static uint32_t kSRVIndexTop;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator = (TextureManager&) = delete;
	//テクスチャデータ
	std::vector<TextureData>textureDatas;
};

