#include "SrvManager.h"

const uint32_t SrvManager::kMaxCount = 512;

SrvManager* SrvManager::instance = nullptr;

// シングルトンインスタンス取得
SrvManager* SrvManager::GetInstance() {
    if (instance == nullptr) {
        instance = new SrvManager;
    }
    return instance;
}

// 初期化
void SrvManager::Initialize(DirectXBase* directxBase) {
    this->directxBase = directxBase;

    // デスクリプタヒープを生成（CBV/SRV/UAV用、最大 kMaxCount、ShaderVisible=true）
    descriptorHeap = directxBase->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxCount, true);

    // デスクリプタ1個分のサイズを取得して記録
    descriptorSize = directxBase->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

// 終了
void SrvManager::Finalize() {
    delete instance;
    instance = nullptr;
}

// SRV作成（2Dテクスチャ用）
void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DirectX::TexMetadata metadata) {
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = metadata.format;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    if (metadata.IsCubemap()) {
        // キューブマップの場合の設定
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.TextureCube.MostDetailedMip = 0;
        srvDesc.TextureCube.MipLevels = UINT_MAX;
        srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
    } else {
        // 通常の2Dテクスチャ
        srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = static_cast<UINT>(metadata.mipLevels);
    }

    directxBase->Getdevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

// SRV作成（Structured Buffer用）
void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    srvDesc.Buffer.NumElements = numElements;
    srvDesc.Buffer.StructureByteStride = structureByteStride;

    directxBase->Getdevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

// SRV作成（RenderTexture用）
void SrvManager::CreateSRVforRenderTexture(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format) {
    D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
    renderTextureSrvDesc.Format = DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;
    renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    renderTextureSrvDesc.Texture2D.MipLevels = 1;

    directxBase->Getdevice()->CreateShaderResourceView(directxBase->GetRenderTextureResource(), &renderTextureSrvDesc, GetCPUDescriptorHandle(srvIndex));
}

// DescriptorHeapをコマンドリストにセット
void SrvManager::PreDraw() {
    ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
    directxBase->Getcommandlist()->SetDescriptorHeaps(1, descriptorHeaps);
}

// GPU側にSRVをセット
void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {
    directxBase->Getcommandlist()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}

// 使用可能か確認
bool SrvManager::Securedcheck() {
    return useIndex < kMaxCount;
}

// SRV用インデックスを確保
uint32_t SrvManager::Allcate() {
    // 上限に達していないか確認
    assert(useIndex < kMaxCount);

    int index = useIndex;
    useIndex++;
    return index;
}

// CPUハンドル取得
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {
    D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    handleCPU.ptr += (descriptorSize * index);
    return handleCPU;
}

// GPUハンドル取得
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {
    D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    handleGPU.ptr += (descriptorSize * index);
    return handleGPU;
}
