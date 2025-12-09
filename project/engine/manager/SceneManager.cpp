#include "SceneManager.h"
#include "Input.h"
#include <cassert>

namespace MyEngine {

    std::unique_ptr<SceneManager> SceneManager::instance_ = nullptr;

    // シングルトンインスタンス取得
    SceneManager* SceneManager::GetInstance() {
        if (!instance_) {
            instance_ = std::make_unique<SceneManager>();
        }
        return instance_.get();
    }

    // 終了処理
    void SceneManager::Finalize() {
        if (scene_) {
            scene_->Finalize();
            scene_.reset();
        }
        instance_.reset();
    }

    // 更新処理
    void SceneManager::Update() {
        // 次のシーンがセットされている場合は切り替え
        if (next_scene_) {
            // 現在のシーンを終了
            if (scene_) {
                scene_->Finalize();
            }

            // シーンを切り替え
            scene_ = std::move(next_scene_);
            next_scene_ = nullptr;

            // 新しいシーンにSceneManagerをセット
            scene_->SetSceneManager(this);

            // 新しいシーンを初期化
            scene_->Initialize();
        }

        // 実行中のシーンを更新
        scene_->Update();
    }

    // 描画
    void SceneManager::Draw() {
        scene_->Draw();
    }

    // 次のシーンを予約
    void SceneManager::ChangeScene(const std::string& scene_name) {
        assert(scene_factory_);        // シーンファクトリが存在することを確認
        assert(next_scene_ == nullptr); // 既に予約済みのシーンがないことを確認

        // 次のシーンを生成して予約
        next_scene_ = scene_factory_->CreateScene(scene_name);
    }
}