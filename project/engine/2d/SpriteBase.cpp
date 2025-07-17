#include "SpriteBase.h"

SpriteBase* SpriteBase::instance = nullptr;

//シングルトンインスタンス
SpriteBase* SpriteBase::GetInstance() {
	if (instance == nullptr) {
		instance = new SpriteBase;
	}
	return instance;
}

//初期化
void SpriteBase::Initialize(DirectXBase*directxBase) {
	directxBase_ =directxBase;
	pso_ = std::make_unique<Pso>();
	pso_->Initialize(directxBase_);
	//グラフィックスパイプラインの生成
	pso_->CreateSpritePipelineState();
}

//終了
void SpriteBase::Finalize() {
	delete instance;
	instance = nullptr;
}

//共通描画設定
void SpriteBase::DrawBaseSet() {
	//ルートシグネチャのセットコマンド
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(pso_->GetSpriteRootSignature());
	//グラフィックスパイプラインステートのセットコマンド
	directxBase_->Getcommandlist()->SetPipelineState(pso_->GetSpriteGraphicsPipelineState());
	//プリミティブトポロジーをセットするコマンド
	// 形状を設定
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}