#include "SpriteBase.h"

namespace MyEngine {

	std::unique_ptr<SpriteBase> SpriteBase::instance = nullptr;

	// シングルトンインスタンスの取得
	SpriteBase* SpriteBase::GetInstance() {
		if (!instance) {
			instance = std::make_unique<SpriteBase>();
		}
		return instance.get();
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
		instance.reset();
	}

	void SpriteBase::DrawBaseSet() {
		// ルートシグネチャを設定（シェーダとのリソースバインド情報）
		directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetSpriteRootSignature());

		// グラフィックスパイプラインステートを設定（描画の各種設定を反映）
		directx_base_->GetCommandList()->SetPipelineState(pso_->GetSpriteGraphicsPipelineState());

		// プリミティブトポロジー（三角形リスト）を設定
		directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}