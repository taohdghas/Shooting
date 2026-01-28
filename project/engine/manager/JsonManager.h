#pragma once
#include "Struct.h"
#include <string>
#include <vector>
#include <memory>
#include <json.hpp>

namespace MyEngine {

    // JSON に関連するクラス
    class JsonManager {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static JsonManager* GetInstance();

        /// <summary>
        /// JSON ファイルを読み込み、LevelData を生成して返す。
        /// </summary>
        std::unique_ptr<LevelData> LoadJsonFile(const std::string& filename);

        /// <summary>
        /// JSONからTransformオブジェクトから準拠・回転・スケールを読み取り、エンジン用に変換して格納する。
        /// </summary>
        void ReadTransform(const nlohmann::json& transform, Vector3& translation, Vector3& rotation, Vector3& scaling);

    public:
        JsonManager() = default;
        ~JsonManager() = default;

    private:
        static std::unique_ptr<JsonManager> instance_;

        static const std::string kDefaultBaseDirectory;
        static const std::string kExtension;
    };
}