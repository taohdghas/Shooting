#include "SceneFactory.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "GameSelectScene.h"
#include "GameClearScene.h"
#include "GameOverScene.h"
#include <unordered_map>
#include <functional>

namespace MyEngine {

    // シーン名に応じたシーンを生成するファクトリメソッド
    std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& scene_name) {
        // シーン名と生成関数のマップ
        static const std::unordered_map<std::string, std::function<std::unique_ptr<BaseScene>()>> scene_map = {
            { "TITLE",  [] { return std::make_unique<TitleScene>(); } },
            { "GAME",   [] { return std::make_unique<GameScene>(); } },
            { "SELECT", [] { return std::make_unique<GameSelectScene>(); } },
            { "CLEAR",  [] { return std::make_unique<GameClearScene>(); } },
            { "OVER",   [] { return std::make_unique<GameOverScene>(); } }
        };

        auto it = scene_map.find(scene_name);
        if (it != scene_map.end()) {
            return it->second();
        }
        // 上記以外はnullptr
        return nullptr;
    }
}