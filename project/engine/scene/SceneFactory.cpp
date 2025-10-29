#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameSelectScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

// シーン名に応じたシーンを生成するファクトリメソッド
std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "TITLE") {
		return std::make_unique<TitleScene>();
	
	}
	else if (sceneName == "GAME") {
		return std::make_unique<GameScene>();
	}
	else if (sceneName == "SELECT") {
		return std::make_unique<GameSelectScene>();
	}
	else if (sceneName == "CLEAR") {
		return std::make_unique<GameClearScene>();
	} 
	else if (sceneName == "OVER") {
		return std::make_unique<GameOverScene>();
	}
	return nullptr;
}