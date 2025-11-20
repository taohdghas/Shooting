#include "SpriteBase.h"

SpriteBase* SpriteBase::instance = nullptr;

// シングルトンインスタンスの取得
SpriteBase* SpriteBase::GetInstance() {
	if (instance == nullptr) {
		instance = new SpriteBase;
	}
	return instance;
}

void SpriteBase::Initialize(DirectXBase* directx_base) {
	// DirectXBaseのポインタを保持
	directx_base_ = directx_base;

	// PSO（パイプラインステートオブジェクト）の生成と初期化
	pso_ = std::make_unique<PipelineStateObject>();
	pso_->Initialize(directx_base_);

	// スプライト描画用のグラフィックスパイプラインを生成
	pso_->CreateSpritePipelineState();
}

void SpriteBase::Finalize() {
	// シングルトンインスタンスの破棄
	delete instance;
	instance = nullptr;
}

void SpriteBase::DrawBaseSet() {
	// ルートシグネチャを設定（シェーダとのリソースバインド情報）
	directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetSpriteRootSignature());

	// グラフィックスパイプラインステートを設定（描画の各種設定を反映）
	directx_base_->GetCommandList()->SetPipelineState(pso_->GetSpriteGraphicsPipelineState());

	// プリミティブトポロジー（三角形リスト）を設定
	directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
