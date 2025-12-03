#include "SceneManager.h"
#include "Input.h"
#include <cassert>

SceneManager* SceneManager::instance = nullptr;

// シングルトンインスタンス取得
SceneManager* SceneManager::GetInstance() {
    if (instance == nullptr) {
        instance = new SceneManager;
    }
    return instance;
}

// 終了処理
void SceneManager::Finalize() {
    // 最後に実行中のシーンを終了
    scene_->Finalize();
    delete instance;
    instance = nullptr;
}

// 更新処理
void SceneManager::Update() {

    // 次のシーンがセットされている場合は切り替え
    if (nextScene_) {
        // 現在のシーンを終了
        if (scene_) {
            scene_->Finalize();
        }

        // シーンを切り替え
        scene_ = std::move(nextScene_);
        nextScene_ = nullptr;

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
void SceneManager::ChangeScene(const std::string& sceneName) {
    assert(sceneFactory_);      // シーンファクトリが存在することを確認
    assert(nextScene_ == nullptr); // 既に予約済みのシーンがないことを確認

    // 次のシーンを生成して予約
    nextScene_ = sceneFactory_->CreateScene(sceneName);
}
