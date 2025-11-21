#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameSelectScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

// シーン名に応じたシーンを生成するファクトリメソッド
std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& scene_name) {

	if (scene_name == "TITLE") {
		return std::make_unique<TitleScene>();
	}
	else if (scene_name == "GAME") {
		return std::make_unique<GameScene>();
	}
	else if (scene_name == "SELECT") {
		return std::make_unique<GameSelectScene>();
	}
	else if (scene_name == "CLEAR") {
		return std::make_unique<GameClearScene>();
	} 
	else if (scene_name == "OVER") {
		return std::make_unique<GameOverScene>();
	}
	// 上記以外はnullptr
	return nullptr;
}