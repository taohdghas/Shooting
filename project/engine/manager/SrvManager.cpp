#include "SrvManager.h"

const uint32_t SrvManager::kMaxCount = 512;

std::unique_ptr<SrvManager> SrvManager::instance_ = nullptr;

// シングルトンインスタンス取得
SrvManager* SrvManager::GetInstance() {
    if (!instance_) {
        instance_ = std::make_unique<SrvManager>();
    }
    return instance_.get();
}

// 初期化
void SrvManager::Initialize(DirectXBase* directx_base) {
    directx_base_ = directx_base;

    // デスクリプタヒープを生成（CBV/SRV/UAV用、最大 kMaxCount、ShaderVisible=true）
    descriptor_heap_ = directx_base_->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxCount, true);

    // デスクリプタ1個分のサイズを取得して記録
    descriptor_size_ = directx_base_->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// 終了
void SrvManager::Finalize() {
    instance_.reset();
}

// SRV作成（2Dテクスチャ用）
void SrvManager::CreateSrvForTexture2D(uint32_t srv_index, ID3D12Resource* p_resource, DirectX::TexMetadata metadata) {
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
    srv_desc.Format = metadata.format;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    if (metadata.IsCubemap()) {
        // キューブマップの場合の設定
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        srv_desc.TextureCube.MostDetailedMip = 0;
        srv_desc.TextureCube.MipLevels = UINT_MAX;
        srv_desc.TextureCube.ResourceMinLODClamp = 0.0f;
    } else {
        // 通常の2Dテクスチャ
        srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srv_desc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);
    }

    directx_base_->GetDevice()->CreateShaderResourceView(p_resource, &srv_desc, GetCpuDescriptorHandle(srv_index));
}

// SRV作成（Structured Buffer用）
void SrvManager::CreateSrvForStructuredBuffer(uint32_t srv_index, ID3D12Resource* p_resource, UINT num_elements, UINT structure_byte_stride) {
    D3D12_SHADER_RESOURCE_VIEW_DESC srv_desc{};
    srv_desc.Format = DXGI_FORMAT_UNKNOWN;
    srv_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srv_desc.Buffer.FirstElement = 0;
    srv_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srv_desc.Buffer.NumElements = num_elements;
    srv_desc.Buffer.StructureByteStride = structure_byte_stride;

    directx_base_->GetDevice()->CreateShaderResourceView(p_resource, &srv_desc, GetCpuDescriptorHandle(srv_index));
}

// SRV作成（RenderTexture用）
void SrvManager::CreateSrvForRenderTexture(uint32_t srv_index, ID3D12Resource* p_resource, DXGI_FORMAT format) {
    D3D12_SHADER_RESOURCE_VIEW_DESC render_texture_srv_desc{};
    render_texture_srv_desc.Format = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
    render_texture_srv_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    render_texture_srv_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    render_texture_srv_desc.Texture2D.MipLevels = 1;

    directx_base_->GetDevice()->CreateShaderResourceView(directx_base_->GetRenderTextureResource(), &render_texture_srv_desc, GetCpuDescriptorHandle(srv_index));
}

// DescriptorHeapをコマンドリストにセット
void SrvManager::PreDraw() {
    ID3D12DescriptorHeap* descriptor_heaps[] = { descriptor_heap_.Get() };
    directx_base_->GetCommandList()->SetDescriptorHeaps(1, descriptor_heaps);
}

// GPU側にSRVをセット
void SrvManager::SetGraphicsRootDescriptorTable(UINT root_parameter_index, uint32_t srv_index) {
    directx_base_->GetCommandList()->SetGraphicsRootDescriptorTable(root_parameter_index, GetGpuDescriptorHandle(srv_index));
}

// 使用可能か確認
bool SrvManager::SecuredCheck() {
    return use_index_ < kMaxCount;
}

// SRV用インデックスを確保
uint32_t SrvManager::Allocate() {
    // 上限に達していないか確認
    assert(use_index_ < kMaxCount);

    int index = use_index_;
    use_index_++;
    return index;
}

// CPUハンドル取得
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCpuDescriptorHandle(uint32_t index) {
    D3D12_CPU_DESCRIPTOR_HANDLE handle_cpu = descriptor_heap_->GetCPUDescriptorHandleForHeapStart();
    handle_cpu.ptr += (descriptor_size_ * index);
    return handle_cpu;
}

// GPUハンドル取得
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGpuDescriptorHandle(uint32_t index) {
    D3D12_GPU_DESCRIPTOR_HANDLE handle_gpu = descriptor_heap_->GetGPUDescriptorHandleForHeapStart();
    handle_gpu.ptr += (descriptor_size_ * index);
    return handle_gpu;
}
