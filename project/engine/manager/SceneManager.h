#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

// シーンを管理するクラス
class SceneManager
{
public:
    /// <summary>
    /// シングルトンインスタンスを取得する。
    /// - 初回呼び出し時にインスタンスを生成して返す。
    /// </summary>
    static SceneManager* GetInstance();
    /// <summary>
    /// 終了処理を行う。
    /// - 現在のシーンを Finalize() してからシングルトンインスタンスを破棄する。
    /// </summary>
    void Finalize();
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// - next_scene_ がセットされていれば現在のシーンを終了し（存在すれば）、next_scene_ を現在のシーンに切り替えて初期化する。
    /// - その後、現在のシーンの Update() を呼び出す。
    /// </summary>
    void Update();
    /// <summary>
    /// 描画処理を行う。
    /// - 現在のシーンの Draw() を呼び出す。
    /// </summary>
    void Draw();
public:
    /// <summary>
    /// 次のシーンを予約する。
    /// - 指定したシーン名に対応するシーンインスタンスをシーンファクトリで生成し、next_scene_ に格納する。
    /// - 呼び出し前に scene_factory_ がセットされていること、および next_scene_ が未設定であることが前提。
    /// </summary>
    void ChangeScene(const std::string& scene_name);
    /// <summary>
    /// シーンファクトリーをセットする。
    /// - SceneManager はこのファクトリーを用いてシーンの生成を行う。
    /// </summary>
    void SetSceneFactory(AbstractSceneFactory* scene_factory) { scene_factory_ = scene_factory; }
private:
    static std::unique_ptr<SceneManager> instance_;
    SceneManager* scene_manager_ = nullptr;
    // 現在のシーン
    std::unique_ptr<BaseScene> scene_;
    // 次のシーン
    std::unique_ptr<BaseScene> next_scene_;
    // シーンファクトリー
    AbstractSceneFactory* scene_factory_ = nullptr;
};
