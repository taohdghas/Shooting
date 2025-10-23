#include "TextureManager.h"
#include "StringUtility.h"

TextureManager* TextureManager::instance = nullptr;

// ImGuiで0番を使用しないため、SRVインデックスは1番から開始
uint32_t TextureManager::kSRVIndexTop = 1;

// シングルトンインスタンスの取得
TextureManager* TextureManager::GetInstance() {
    if (instance == nullptr) {
        instance = new TextureManager;
    }
    return instance;
}

// シングルトンの解放
void TextureManager::Finalize() {
    delete instance;
    instance = nullptr;
}

// 初期化処理
void TextureManager::Initialize(DirectXBase* directxBase, SrvManager* srvManager) {
    directxBase_ = directxBase;
    srvManager_ = srvManager;
}

// テクスチャファイルの読み込み
void TextureManager::LoadTexture(const std::string& filePath) {
    // すでに読み込まれている場合は何もしない
    if (textureDatas.contains(filePath)) {
        return;
    }

    // テクスチャ数上限チェック（SrvManagerの安全確認）
    assert(srvManager_->Securedcheck());

    DirectX::ScratchImage image{};
    std::wstring filePathW = StringUtility::ConvertString(filePath);

    // .dds形式かそれ以外かで読み込み方法を切り替える
    HRESULT hr;
    if (filePathW.ends_with(L".dds")) {
        hr = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
    } else {
        hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    }
    assert(SUCCEEDED(hr));

    DirectX::ScratchImage mipImages{};
    // 圧縮テクスチャかどうかを判定
    if (DirectX::IsCompressed(image.GetMetadata().format)) {
        // 圧縮済みテクスチャはそのまま使用
        mipImages = std::move(image);
    } else {
        // 非圧縮の場合はミップマップを生成
        hr = DirectX::GenerateMipMaps(
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            DirectX::TEX_FILTER_SRGB,
            4,
            mipImages
        );
        assert(SUCCEEDED(hr));
    }

    // テクスチャデータを登録
    TextureData& textureData = textureDatas[filePath];
    textureData.metadata = mipImages.GetMetadata();
    textureData.resource = directxBase_->CreateTextureResource(textureData.metadata);
    textureData.intermediateResource = directxBase_->UploadTextureData(textureData.resource, mipImages);

    // SRVインデックスの計算と割り当て
    uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;
    textureData.srvIndex = srvManager_->Allcate();
    textureData.srvHandleCPU = srvManager_->GetCPUDescriptorHandle(textureData.srvIndex);
    textureData.srvHandleGPU = srvManager_->GetGPUDescriptorHandle(textureData.srvIndex);

    // SRV作成
    srvManager_->CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata);
}

// ファイルパスからSRVインデックスを取得
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath) {
    auto it = textureDatas.find(filePath);
    if (it != textureDatas.end()) {
        return it->second.srvIndex;
    }
    return 0; // 読み込まれていない場合は0を返す
}

// ファイルパスからメタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath) {
    assert(srvManager_->Securedcheck()); // 安全確認
    TextureData& textureData = textureDatas[filePath];
    return textureData.metadata;
}

// ファイルパスからSRVインデックスを取得
uint32_t TextureManager::GetSrvIndex(const std::string& filePath) {
    TextureData& textureData = textureDatas[filePath];
    return textureData.srvIndex;
}

// ファイルパスからGPU側SRVハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& filePath) {
    assert(srvManager_->Securedcheck()); // 安全確認
    TextureData& textureData = textureDatas[filePath];
    return textureData.srvHandleGPU;
}
