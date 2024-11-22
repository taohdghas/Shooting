#include "SrvManager.h"

const uint32_t SrvManager::kMaxCount = 512;

//初期化
void SrvManager::Initialize(DirectXBase* directxBase) {
	this->directxBase = directxBase;
	//デスクリプタヒープの生成
	descriptorHeap = directxBase->CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, kMaxCount,true);
	//デスクリプタ1個分のサイズを取得して記録
	descriptorSize = directxBase->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}
//SRV生成(テクスチャ用)
void SrvManager::CreateSRVforTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT Format, UINT MipLevels) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//SRVの設定
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = MipLevels;

	directxBase->Getdevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}
//SRV生成(Structured Buffer用)
void SrvManager::CreateSRVforStructuredBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//SRVの設定
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	directxBase->Getdevice()->CreateShaderResourceView(pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}
//ヒープセットコマンド
void SrvManager::PreDraw() {
	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap.Get() };
	directxBase->Getcommandlist()->SetDescriptorHeaps(1, descriptorHeaps);
}
//SRVセットコマンド
void SrvManager::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, uint32_t srvIndex) {
	directxBase->Getcommandlist()->SetGraphicsRootDescriptorTable(RootParameterIndex, GetGPUDescriptorHandle(srvIndex));
}
//確保可能かチェック
bool SrvManager::Securedcheck() {
	if (useIndex < kMaxCount) {
		return true;
	}
	return false;
}
uint32_t SrvManager::Allccate() {
	//上限に達していないかチェック
	assert(useIndex < kMaxCount);

	//returnする番号を一旦記録しておく
	int index = useIndex;
	//次のため番号を進める
	useIndex++;
	//上で記録した番号をreturn
	return index;
}
//CPUハンドル計算
D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUDescriptorHandle(uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}
//GPUハンドル計算
D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUDescriptorHandle(uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}