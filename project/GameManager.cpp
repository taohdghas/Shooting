#include "GameManager.h"

GameManager::GameManager() {
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[STAGE] = std::make_unique<StageScene>();

	preSceneNo_ = TITLE;
	currentSceneNo_ = TITLE;
	sceneArr_[currentSceneNo_]->Initialize();
}

GameManager::~GameManager(){}

//ゲームループ呼び出し
int GameManager::Run() {
	if (preSceneNo_ != currentSceneNo_) {
		sceneArr_[currentSceneNo_]->Initialize();
		preSceneNo_ = currentSceneNo_;
	}
	sceneArr_[currentSceneNo_]->Update();
	sceneArr_[currentSceneNo_]->Draw();

	currentSceneNo_ = iscene_->GetSceneNo();
	return currentSceneNo_;
}
