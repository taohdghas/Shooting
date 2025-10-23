#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameSelectScene.h"
#include "GameClearScene.h"

// シーン名に応じたシーンを生成するファクトリメソッド
std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {

    // TITLEシーンを生成
    if (sceneName == "TITLE") {
        return std::make_unique<TitleScene>();
    }
    // GAMEシーンを生成
    else if (sceneName == "GAME") {
        return std::make_unique<GameScene>();
    }
    // ゲーム選択シーンを生成
    else if (sceneName == "SELECT") {
        return std::make_unique<GameSelectScene>();
    }
    // ゲームクリアシーンを生成
    else if (sceneName == "CLEAR") {
        return std::make_unique<GameClearScene>();
    }

    // 指定されたシーン名が存在しない場合は nullptr を返す
    return nullptr;
}
