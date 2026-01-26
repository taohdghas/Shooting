#include "JsonManager.h"
#include <fstream>
#include <cassert>
#include <json.hpp>

namespace MyEngine {

    std::unique_ptr<JsonManager> JsonManager::instance_ = nullptr;

    // シングルトンインスタンス取得
    JsonManager* JsonManager::GetInstance() {
        if (!instance_) {
            instance_ = std::make_unique<JsonManager>();
        }
        return instance_.get();
    }

    // JSONファイルを読み込んでLevelDataを生成
    std::unique_ptr<LevelData> JsonManager::LoadJsonFile(const std::string& filename) {
        // フルパスを生成
        const std::string full_path = "resources/levels/" + filename + ".json";

        // ファイルストリームを開く
        std::ifstream file(full_path);
        if (file.fail()) {
            // ファイルが開けなければアサート
            assert(0);
        }

        // JSON文字列をパース
        nlohmann::json deserialized_json;
        file >> deserialized_json;

        // 正しいレベルデータか簡易チェック
        assert(deserialized_json.is_object());
        assert(deserialized_json.contains("name"));
        assert(deserialized_json["name"].is_string());
        std::string name = deserialized_json["name"].get<std::string>();
        assert(name.compare("scene") == 0);

        // レベルデータ格納用インスタンス生成
        auto level_data = std::make_unique<LevelData>();

        // "objects" 配列を走査
        for (nlohmann::json& object_json : deserialized_json["objects"]) {
            assert(object_json.contains("type"));

            // 無効フラグが立っている場合はスキップ
            if (object_json.contains("disabled_flag") && object_json["disabled_flag"].get<bool>()) {
                continue;
            }

            std::string type = object_json["type"].get<std::string>();

            // 種別ごとの処理
            if (type.compare("MESH") == 0) {
                level_data->objects.emplace_back(LevelData::ObjectData{});
                LevelData::ObjectData& object_data = level_data->objects.back();

                if (object_json.contains("file_name")) {
                    object_data.fileName = object_json["file_name"];
                }

                ReadTransform(object_json["transform"], object_data.translation, object_data.rotation, object_data.scaling);

            } else if (type.compare("PlayerSpawn") == 0) {
                PlayerSpawnData player_spawn{};
                ReadTransform(object_json["transform"], player_spawn.translation, player_spawn.rotation, player_spawn.scaling);
                level_data->players.push_back(player_spawn);

            } else if (type.compare("EnemySpawn") == 0) {
                EnemySpawnData enemy_spawn{};
                enemy_spawn.name = object_json["name"].get<std::string>();
                enemy_spawn.fileName = object_json["file_name"].get<std::string>();
                ReadTransform(object_json["transform"], enemy_spawn.translation, enemy_spawn.rotation, enemy_spawn.scaling);
                level_data->enemies.push_back(enemy_spawn);
            } else if (type.compare("BossSpawn") == 0) {
                BossSpawnData boss_spawn{};

                boss_spawn.name = object_json["name"].get<std::string>();
                boss_spawn.fileName = object_json["file_name"].get<std::string>();

                ReadTransform(
                    object_json["transform"],
                    boss_spawn.translation,
                    boss_spawn.rotation,
                    boss_spawn.scaling
                );

                level_data->bosses.push_back(boss_spawn);
            }

            // 子オブジェクトを再帰的に処理する予定
            if (object_json.contains("children")) {
                // 未実装
            }
        }

        // rails 配列を読み込む
        if (deserialized_json.contains("rails")) {
            for (auto& rail_json : deserialized_json["rails"]) {
                EnemyRailData rail;
                rail.name = rail_json["name"].get<std::string>();
                rail.closed = rail_json.contains("closed") && rail_json["closed"].get<bool>();

                // control_points
                for (auto& point : rail_json["control_points"]) {
                    rail.controlPoints.push_back({
                        (float)point[0],
                        (float)point[2],
                        (float)point[1]
                        });
                }

                // rail_point_names
                for (auto& name : rail_json["rail_point_names"]) {
                    rail.railPointNames.push_back(name.get<std::string>());
                }

                // owner に紐付け
                std::string owner = rail_json["owner"].get<std::string>();
                for (auto& enemy : level_data->enemies) {
                    if (owner == enemy.name) {
                        enemy.rails.push_back(rail);
                    }
                }
            }
        }


        return level_data;
    }

    void JsonManager::ReadTransform(const nlohmann::json& transform, Vector3& translation, Vector3& rotation, Vector3& scaling) {
        // translation: Y/Z入れ替え
        translation.x = (float)transform["translation"][0];
        translation.y = (float)transform["translation"][2];
        translation.z = (float)transform["translation"][1];

        // rotation: Y/Z入れ替え＋符号反転
        rotation.x = -(float)transform["rotation"][0];
        rotation.y = -(float)transform["rotation"][2];
        rotation.z = -(float)transform["rotation"][1];

        // scaling: Y/Z入れ替え
        scaling.x = (float)transform["scaling"][0];
        scaling.y = (float)transform["scaling"][2];
        scaling.z = (float)transform["scaling"][1];
    }
}