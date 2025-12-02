#include "TextureManager.h"
#include "StringUtility.h"

std::unique_ptr<TextureManager> TextureManager::instance_ = nullptr;

// ImGuiで0番を使用しないため、SRVインデックスは1番から開始
uint32_t TextureManager::k_srv_index_top_ = 1;

// シングルトンインスタンスの取得
TextureManager* TextureManager::GetInstance() {
    if (!instance_) {
        instance_ = std::make_unique<TextureManager>();
    }
    return instance_.get();
}

// シングルトンの解放
void TextureManager::Finalize() {
    instance_.reset();
}

// 初期化処理
void TextureManager::Initialize(DirectXBase* directx_base, SrvManager* srv_manager) {
    directx_base_ = directx_base;
    srv_manager_ = srv_manager;
}

// テクスチャファイルの読み込み
void TextureManager::LoadTexture(const std::string& file_path) {
    // すでに読み込まれている場合は何もしない
    if (texture_datas_.contains(file_path)) {
        return;
    }

    // テクスチャ数上限チェック（SrvManagerの安全確認）
    assert(srv_manager_->SecuredCheck());

    DirectX::ScratchImage image{};
    std::wstring file_path_w = string_utility::ConvertString(file_path);

    // .dds形式かそれ以外かで読み込み方法を切り替える
    HRESULT hr;
    if (file_path_w.ends_with(L".dds")) {
        hr = DirectX::LoadFromDDSFile(file_path_w.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
    } else {
        hr = DirectX::LoadFromWICFile(file_path_w.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    }
    assert(SUCCEEDED(hr));

    DirectX::ScratchImage mip_images{};
    // 圧縮テクスチャかどうかを判定
    if (DirectX::IsCompressed(image.GetMetadata().format)) {
        // 圧縮済みテクスチャはそのまま使用
        mip_images = std::move(image);
    } else {
        // 非圧縮の場合はミップマップを生成
        hr = DirectX::GenerateMipMaps(
            image.GetImages(),
            image.GetImageCount(),
            image.GetMetadata(),
            DirectX::TEX_FILTER_SRGB,
            4,
            mip_images
        );
        assert(SUCCEEDED(hr));
    }

    // テクスチャデータを登録
    TextureData& texture_data = texture_datas_[file_path];
    texture_data.metadata_ = mip_images.GetMetadata();
    texture_data.resource_ = directx_base_->CreateTextureResource(texture_data.metadata_);
    texture_data.intermediate_resource_ = directx_base_->UploadTextureData(texture_data.resource_, mip_images);

    // SRVインデックスの計算と割り当て
    texture_data.srv_index_ = srv_manager_->Allocate();
    texture_data.srv_handle_cpu_ = srv_manager_->GetCpuDescriptorHandle(texture_data.srv_index_);
    texture_data.srv_handle_gpu_ = srv_manager_->GetGpuDescriptorHandle(texture_data.srv_index_);

    // SRV作成
    srv_manager_->CreateSrvForTexture2D(texture_data.srv_index_, texture_data.resource_.Get(), texture_data.metadata_);
}

// ファイルパスからSRVインデックスを取得
uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& file_path) {
    auto it = texture_datas_.find(file_path);
    if (it != texture_datas_.end()) {
        return it->second.srv_index_;
    }
    return 0; // 読み込まれていない場合は0を返す
}

// ファイルパスからメタデータを取得
const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& file_path) {
    assert(srv_manager_->SecuredCheck()); // 安全確認
    TextureData& texture_data = texture_datas_[file_path];
    return texture_data.metadata_;
}

// ファイルパスからSRVインデックスを取得
uint32_t TextureManager::GetSrvIndex(const std::string& file_path) {
    TextureData& texture_data = texture_datas_[file_path];
    return texture_data.srv_index_;
}

// ファイルパスからGPU側SRVハンドルを取得
D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSrvHandleGPU(const std::string& file_path) {
    assert(srv_manager_->SecuredCheck()); // 安全確認
    TextureData& texture_data = texture_datas_[file_path];
    return texture_data.srv_handle_gpu_;
}
