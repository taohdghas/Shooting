#pragma once
#include "Struct.h"
#include <string>
#include <vector>
#include <memory>
#include <json.hpp>

// JSON に関連するクラス
class JsonManager {
public:
    /// <summary>
    /// シングルトンインスタンスを取得する。
    /// - 初回呼び出し時に内部でインスタンスを生成して返す。
    /// </summary>
    /// <returns>JsonManager の単一インスタンスへのポインタ。</returns>
    static JsonManager* GetInstance();

    /// <summary>
    /// JSON ファイルを読み込み、LevelData を生成して返す。
    /// - 実際の実装では "resources/levels/" + filename + ".json" をフルパスとして開く。
    /// - ファイル存在チェックおよび JSON 構造の検証（"name" == "scene"）を行う（失敗時は assert）。
    /// - "objects" 配列を走査し、各オブジェクトの "type" に応じて以下を処理する:
    ///   - "MESH"       : Model 用の ObjectData を追加し、"file_name" と transform(translation/rotation/scaling) を読み取る。
    ///     - 注意: JSON 内の座標系からエンジン用に Y/Z を入れ替えたり回転に符号を付け替える処理がある。
    ///   - "PlayerSpawn": PlayerSpawnData を作成して players 配列に追加する（transform 読み取り、Y/Z 入れ替え等）。
    ///   - "EnemySpawn" : EnemySpawnData を作成して enemies 配列に追加する（transform 読み取り、Y/Z 入れ替え等）。
    /// - 無効化フラグ ("disabled_flag") が true のオブジェクトはスキップする。
    /// - 読み込んだデータは動的に確保された LevelData* として返される（呼び出し側で破棄する責務）。
    /// </summary>
    /// <param name="filename">レベル名（拡張子・ディレクトリは不要、例: "level01"）。</param>
    /// <returns>新しく確保された LevelData*（失敗時は assert により停止する実装）。</returns>
    LevelData* LoadJsonFile(const std::string& filename);

    /// <summary>
    /// JSONのtransformオブジェクトから座標・回転・スケールを読み取り、エンジン用に変換して格納する。
    /// - translation, rotation, scaling 各要素のY/Z入れ替えや回転の符号反転も行う。
    /// </summary>
    /// <param name="transform">JSONのtransformオブジェクト</param>
    /// <param name="translation">変換後の平行移動ベクトル（出力）</param>
    /// <param name="rotation">変換後の回転ベクトル（出力）</param>
    /// <param name="scaling">変換後のスケールベクトル（出力）</param>
    void ReadTransform(const nlohmann::json& transform, Vector3& translation, Vector3& rotation, Vector3& scaling);

public:
    JsonManager() = default;
    ~JsonManager() = default;

private:
    static std::unique_ptr<JsonManager> instance_;

    static const std::string kDefaultBaseDirectory;
    static const std::string kExtension;
};
