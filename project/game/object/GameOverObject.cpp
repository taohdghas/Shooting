#include "GameOverObject.h"
#include "ImGuiManager.h"

//初期化
void GameOverObject::Initialize() {
	//ゲームオーアー字のオブジェクト
	gameover = std::make_unique<Object3d>();
	gameover->Initialize(Object3dBase::GetInstance());
	gameover->SetModel("gameover.obj");
	gameover->SetScale({ 1.5f,1.5f,1.5f });
	gameover->SetTranslate({ -1.2f,0.8f,1.0f });
	//retryのオブジェクト
	retry = std::make_unique<Object3d>();
	retry->Initialize(Object3dBase::GetInstance());
	retry->SetModel("retry.obj");
	retry->SetScale({ 0.5f,0.5f,0.5f });
	retry->SetTranslate({ 1.1f,-1.5f,1.0f });
}
//更新
void GameOverObject::Update() {
	//ゲームオーアー字のオブジェクト
	gameover->Update();
	//retryのオブジェクト
	retry->Update();
}
//描画
void GameOverObject::Draw() {
	//ゲームオーアー字のオブジェクト
	gameover->Draw();
	//retryのオブジェクト
	retry->Draw();
}
//デバック
void GameOverObject::Debug() {}