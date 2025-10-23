#include "JsonManager.h"
#include <fstream>
#include <cassert>
#include <json.hpp>

JsonManager* JsonManager::instance = nullptr;

// シングルトンインスタンス取得
JsonManager* JsonManager::GetInstance() {
    if (instance == nullptr) {
        instance = new JsonManager;
    }
    return instance;
}

// JSONファイルを読み込んでLevelDataを生成
LevelData* JsonManager::LoadJsonFile(const std::string& filename) {
    // フルパスを生成
    const std::string fullpath = "resources/levels/" + filename + ".json";

    // ファイルストリームを開く
    std::ifstream file(fullpath);
    if (file.fail()) {
        // ファイルが開けなければアサート
        assert(0);
    }

    // JSON文字列をパース
    nlohmann::json deserialized;
    file >> deserialized;

    // 正しいレベルデータか簡易チェック
    assert(deserialized.is_object());
    assert(deserialized.contains("name"));
    assert(deserialized["name"].is_string());
    std::string name = deserialized["name"].get<std::string>();
    assert(name.compare("scene") == 0);

    // レベルデータ格納用インスタンス生成
    LevelData* levelData = new LevelData();

    // "objects" 配列を走査
    for (nlohmann::json& object : deserialized["objects"]) {
        assert(object.contains("type"));

        // 無効フラグが立っている場合はスキップ
        if (object.contains("disabled_flag") && object["disabled_flag"].get<bool>()) {
            continue;
        }

        std::string type = object["type"].get<std::string>();

        // 種別ごとの処理
        if (type.compare("MESH") == 0) {
            // オブジェクトを追加
            levelData->objects.emplace_back(LevelData::ObjectData{});
            LevelData::ObjectData& objectData = levelData->objects.back();

            // ファイル名が存在すれば設定
            if (object.contains("file_name")) {
                objectData.fileName = object["file_name"];
            }

            // トランスフォームを読み込み
            nlohmann::json& transform = object["transform"];
            objectData.translation.x = (float)transform["translation"][0];
            objectData.translation.y = (float)transform["translation"][2];
            objectData.translation.z = (float)transform["translation"][1];

            objectData.rotation.x = -(float)transform["rotation"][0];
            objectData.rotation.y = -(float)transform["rotation"][2];
            objectData.rotation.z = -(float)transform["rotation"][1];

            objectData.scaling.x = (float)transform["scaling"][0];
            objectData.scaling.y = (float)transform["scaling"][2];
            objectData.scaling.z = (float)transform["scaling"][1];
        } else if (type.compare("PlayerSpawn") == 0) {
            PlayerSpawnData playerSpawn{};

            // トランスフォーム読み込み
            nlohmann::json& transform = object["transform"];
            playerSpawn.translation.x = (float)transform["translation"][0];
            playerSpawn.translation.y = (float)transform["translation"][2];
            playerSpawn.translation.z = (float)transform["translation"][1];

            playerSpawn.rotation.x = -(float)transform["rotation"][0];
            playerSpawn.rotation.y = -(float)transform["rotation"][2];
            playerSpawn.rotation.z = -(float)transform["rotation"][1];

            playerSpawn.scaling.x = (float)transform["scaling"][0];
            playerSpawn.scaling.y = (float)transform["scaling"][2];
            playerSpawn.scaling.z = (float)transform["scaling"][1];

            levelData->players.push_back(playerSpawn);
        } else if (type.compare("EnemySpawn") == 0) {
            EnemySpawnData enemySpawn{};

            // トランスフォーム読み込み
            nlohmann::json& transform = object["transform"];
            enemySpawn.translation.x = (float)transform["translation"][0];
            enemySpawn.translation.y = (float)transform["translation"][2];
            enemySpawn.translation.z = (float)transform["translation"][1];

            enemySpawn.rotation.x = -(float)transform["rotation"][0];
            enemySpawn.rotation.y = -(float)transform["rotation"][2];
            enemySpawn.rotation.z = -(float)transform["rotation"][1];

            enemySpawn.scaling.x = (float)transform["scaling"][0];
            enemySpawn.scaling.y = (float)transform["scaling"][2];
            enemySpawn.scaling.z = (float)transform["scaling"][1];

            levelData->enemies.push_back(enemySpawn);
        }

        // 子オブジェクトを再帰的に処理する予定
        if (object.contains("children")) {
            // 未実装
        }
    }

    return levelData;
}
