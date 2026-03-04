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
        /// シングルトンインスタンスを取得
        /// <returns>JsonManagerのインスタンス（ポインタ）</returns>
        /// </summary>
        static JsonManager* GetInstance();

        /// <summary>
        /// JSON ファイルを読み込み、LevelData を生成して返す
        /// <param name="filename">JSONファイルのパス（std::string）</param>
        /// <returns>生成されたLevelDataのunique_ptr</returns>
        /// </summary>
        std::unique_ptr<LevelData> LoadJsonFile(const std::string& filename);

        /// <summary>
        /// JSONからTransformオブジェクトから座標・回転・スケールを読み取り、エンジン用に変換して格納
        /// <param name="transform">JSONのTransformオブジェクト</param>
        /// <param name="translation">格納先の平行移動（Vector3）</param>
        /// <param name="rotation">格納先の回転（Vector3）</param>
        /// <param name="scaling">格納先のスケール（Vector3）</param>
        /// <returns>なし</returns>
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