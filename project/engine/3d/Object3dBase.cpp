#include "Object3dBase.h"

namespace MyEngine {

	std::unique_ptr<Object3dBase> Object3dBase::instance = nullptr;

	// シングルトンインスタンスの取得
	Object3dBase* Object3dBase::GetInstance() {
		if (!instance) {
			instance = std::make_unique<Object3dBase>();
		}
		return instance.get();
	}

	void Object3dBase::Initialize(DirectXBase* directx_base) {
		// DirectXBaseのポインタを保持
		directx_base_ = directx_base;

		// PSO（パイプラインステートオブジェクト）の生成と初期化
		pso_ = std::make_unique<PipelineStateObject>();
		pso_->Initialize(directx_base_);

		// 3Dオブジェクト描画用のグラフィックスパイプラインを生成
		pso_->CreatePipelineState();
	}

	void Object3dBase::Finalize() {
		// シングルトンインスタンスの破棄
		instance.reset();
	}

	void Object3dBase::DrawBaseSet() {
		// ルートシグネチャを設定
		directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetRootSignature());

		// グラフィックスパイプラインステートを設定
		directx_base_->GetCommandList()->SetPipelineState(pso_->GetGraphicsPipelineState());

		// プリミティブトポロジー（三角形リスト）を設定
		directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}