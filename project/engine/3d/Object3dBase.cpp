#include "Object3dBase.h"

Object3dBase* Object3dBase::instance = nullptr;

//シングルトンインスタンス
Object3dBase* Object3dBase::GetInstance() {
	if (instance == nullptr) {
		instance = new Object3dBase;
	}
	return instance;
}

//初期化
void Object3dBase::Initialize(DirectXBase*directxBase) {
	directxBase_ = directxBase;
	pso_ = std::make_unique<Pso>();
	pso_->Initialize(directxBase_);
	pso_->CreatePipelineState();
}

//終了
void Object3dBase::Finalize() {
	delete instance;
	instance = nullptr;
}


//共通描画設定
void Object3dBase::DrawBaseSet() {

	//ルートシグネチャのセットコマンド
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(pso_->GetRootSignature());
	//グラフィックスパイプラインステートのセットコマンド
	directxBase_->Getcommandlist()->SetPipelineState(pso_->GetGraphicsPipelineState());
	//プリミティブトポロジーをセットするコマンド
	// 形状を設定
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}