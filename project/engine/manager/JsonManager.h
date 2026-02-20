#pragma once
#include "Struct.h"
#include <json.hpp>
#include <memory>
#include <string>
#include <vector>

namespace MyEngine {

    // JSON に関連するクラス
    /// <summary>
    /// JSONファイルの読み込み・パースおよびエンジン用データへの変換
    /// Transform情報（座標・回転・スケール）のJSONからの抽出とVector3等への格納
    /// ゲームレベルやオブジェクト配置データのロード処理の一元化
    /// シングルトンによるインスタンス管理
    /// JSONファイルのパスや拡張子等の管理
    /// </summary>
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
        /// JSONからTransformオブジェクトから座標・回転・スケールを読み取り、エンジン用に変換して格納する。
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