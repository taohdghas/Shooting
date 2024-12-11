#include "Enemy.h"

//初期化
void Enemy::Initialize() {
	//モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize(directxBase_);
	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	//オブジェクト初期化
	object3d_->Initialize(object3dBase_);
	object3d_->SetModel("plane.obj");
}
//更新
void Enemy::Update() {
	object3d_->Update();
}
//描画
void Enemy::Draw() {
	object3d_->Draw();
}