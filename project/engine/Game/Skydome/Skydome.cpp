#include "Skydome.h"

//初期化
void Skydome::Initialize() {
	//モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(directxBase_);
	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("axis.obj");
	//オブジェクト初期化
	object3d_->Initialize(object3dBase_);
	object3d_->SetModel("axis.obj");
}
//更新
void Skydome::Update() {
	object3d_->Update();
}
//描画
void Skydome::Draw() {
	object3d_->Draw();
}
