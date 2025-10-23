#include "Object3dBase.h"

Object3dBase* Object3dBase::instance = nullptr;

// シングルトンインスタンスの取得
Object3dBase* Object3dBase::GetInstance() {
	if (instance == nullptr) {
		instance = new Object3dBase;
	}
	return instance;
}

// 初期化
void Object3dBase::Initialize(DirectXBase* directxBase) {
	directxBase_ = directxBase;

	// PSO（パイプラインステートオブジェクト）の生成と初期化
	pso_ = std::make_unique<Pso>();
	pso_->Initialize(directxBase_);

	// 3Dオブジェクト用のパイプラインステートを作成
	pso_->CreatePipelineState();
}

// 終了処理（シングルトンインスタンスの破棄）
void Object3dBase::Finalize() {
	delete instance;
	instance = nullptr;
}

// 共通描画設定
void Object3dBase::DrawBaseSet() {
	// ルートシグネチャを設定（シェーダとリソースの結び付け定義）
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(pso_->GetRootSignature());

	// パイプラインステートを設定（描画時の設定情報をまとめたもの）
	directxBase_->Getcommandlist()->SetPipelineState(pso_->GetGraphicsPipelineState());

	// プリミティブトポロジーを設定（三角形リストとして描画）
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
