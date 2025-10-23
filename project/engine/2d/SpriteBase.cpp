#include "SpriteBase.h"

SpriteBase* SpriteBase::instance = nullptr;

// シングルトンインスタンスの取得
SpriteBase* SpriteBase::GetInstance() {
	if (instance == nullptr) {
		instance = new SpriteBase;
	}
	return instance;
}

// 初期化
void SpriteBase::Initialize(DirectXBase* directxBase) {
	directxBase_ = directxBase;

	// PSO（パイプラインステートオブジェクト）の生成と初期化
	pso_ = std::make_unique<Pso>();
	pso_->Initialize(directxBase_);

	// スプライト描画用のグラフィックスパイプラインを生成
	pso_->CreateSpritePipelineState();
}

// 終了処理（シングルトンインスタンスの破棄）
void SpriteBase::Finalize() {
	delete instance;
	instance = nullptr;
}

// 共通描画設定
void SpriteBase::DrawBaseSet() {
	// ルートシグネチャを設定（シェーダとのリソースバインド情報）
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(pso_->GetSpriteRootSignature());

	// グラフィックスパイプラインステートを設定（描画の各種設定を反映）
	directxBase_->Getcommandlist()->SetPipelineState(pso_->GetSpriteGraphicsPipelineState());

	// プリミティブトポロジー（三角形リスト）を設定
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
