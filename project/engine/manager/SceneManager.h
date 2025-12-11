#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

namespace MyEngine {

    // シーンを管理するクラス
    class SceneManager
    {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static SceneManager* GetInstance();
        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();
        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// </summary>
        void Update();
        /// <summary>
        /// 描画処理を行う。
        /// </summary>
        void Draw();
    public:
        /// <summary>
        /// 次のシーンを予約する。
        /// </summary>
        void ChangeScene(const std::string& scene_name);
        /// <summary>
        /// シーンファクトリーをセットする。
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
}