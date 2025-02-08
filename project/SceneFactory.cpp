#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"

//シーン生成
std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "TITLE") {
		return std::make_unique<TitleScene>();
	
	}
	else if (sceneName == "GAME") {
		return std::make_unique<GameScene>();
	}
	return nullptr;
}