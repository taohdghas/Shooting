#include "Object3dBase.h"

Object3dBase* Object3dBase::instance = nullptr;

// シングルトンインスタンスの取得
Object3dBase* Object3dBase::GetInstance() {
	if (instance == nullptr) {
		instance = new Object3dBase;
	}
	return instance;
}

void Object3dBase::Initialize(DirectXBase* directx_base) {
	// DirectXBaseのポインタを保持
	directx_base_ = directx_base;

	// PSO（パイプラインステートオブジェクト）の生成と初期化
	pso_ = std::make_unique<PipelineStateObject>();
	pso_->Initialize(directx_base_);

	// 3Dオブジェクト用のパイプラインステートを作成
	pso_->CreatePipelineState();
}

void Object3dBase::Finalize() {
	// シングルトンインスタンスの破棄
	delete instance;
	instance = nullptr;
}

void Object3dBase::DrawBaseSet() {
	// ルートシグネチャを設定（シェーダとリソースの結び付け定義）
	directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetRootSignature());

	// パイプラインステートを設定（描画時の設定情報をまとめたもの）
	directx_base_->GetCommandList()->SetPipelineState(pso_->GetGraphicsPipelineState());

	// プリミティブトポロジーを設定（三角形リストとして描画）
	directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
