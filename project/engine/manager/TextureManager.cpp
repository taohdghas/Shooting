#include "TextureManager.h"
#include "StringUtility.h"

TextureManager* TextureManager::instance = nullptr;

//ImGuiで0盤を使用するため、1番から使用
uint32_t TextureManager::kSRVIndexTop = 1;

//シングルインスタンスの取得
TextureManager* TextureManager::GetInstance() {
	if (instance == nullptr) {
		instance = new TextureManager;
	}
	return instance;
}
//終了
void TextureManager::Finalize() {
	delete instance;
	instance = nullptr;
}
//初期化
void TextureManager::Initialize(DirectXBase*directxBase, SrvManager* srvManager) {
	directxBase_ = directxBase;
	srvManager_ = srvManager;
}
//テクスチャファイルの読み込み
void TextureManager::LoadTexture(const std::string& filePath) {

	//読み込み済みテクスチャを検索
	if (textureDatas.contains(filePath)) {
		return;
	}
	//基盤？
	//テクスチャ枚数上限チェック
	assert(srvManager_->Securedcheck());

	DirectX::ScratchImage image{};
	//テクスチャファイルを読んでプログラムで抑えるようにする
	std::wstring filePathW = StringUtility::ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	//ミップマップの作成
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));

	/*
	//テクスチャデータを追加
	textureDatas.resize(textureDatas.size() + 1);
	//追加したテクスチャデータの参照を取得する
	TextureData& textureData = textureDatas.back();
	*/
	//テクスチャデータを追加して書きこむ
	TextureData& textureData = textureDatas[filePath];

	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = directxBase_->CreateTextureResource(textureData.metadata);
	textureData.intermediateResource = directxBase_->UploadTextureData(textureData.resource, mipImages);

    //テクスチャデータの要素数番号をSRVのインデックスとする
	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	textureData.srvIndex = srvManager_->Allccate();
	textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
	textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);
	/*
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//SRVの設定
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	//設定を元にSRVの生成
	directxBase_->Getdevice()->CreateShaderResourceView(textureData.resource.Get(), &srvDesc, textureData.srvHandleCPU);
	*/
	srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(),
		textureData.metadata.format, UINT(textureData.metadata.mipLevels));
}
//SRVインデックスの開始番号
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
	//読み込み済みテクスチャデータを検索
	auto it = std::find_if(textureDatas.begin(), textureDatas.end(),
		[&](TextureData& textureData) {
			return textureData.filePath == filePath;
		});
	if (it != textureDatas.end()) {
		//読み込み済みなら要素番号を返す
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

//メタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string&filePath) {
	//範囲外指定違反チェック
	assert(srvManager_->Securedcheck());

	TextureData& textureData = textureDatas[filePath];
	return textureData.metadata;
}
//SRVインデックスの取得
uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {
	TextureData& textureData = textureDatas[filePath];
	return textureData.srvIndex;
}
//GPUハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {
	//範囲外指定違反チェック
	assert(srvManager_->Securedcheck());

	TextureData& textureData = textureDatas[filePath];
	return textureData.srvHandleGPU;
}