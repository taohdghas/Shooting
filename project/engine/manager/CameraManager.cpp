#include "CameraManager.h"

CameraManager* CameraManager::instance = nullptr;

// シングルトンインスタンス取得
CameraManager* CameraManager::GetInstance() {
    if (instance == nullptr) {
        instance = new CameraManager;
    }
    return instance;
}

// 初期化
void CameraManager::Initialize() {
    cameras.clear();      // 登録済みカメラをクリア
    activeCamera = nullptr; // アクティブカメラをリセット
}

// 終了
void CameraManager::Finalize() {
    cameras.clear();       // 登録済みカメラをクリア
    delete instance;       // シングルトン解放
    instance = nullptr;
}

// カメラを追加
void CameraManager::AddCamera(const std::string& name, Camera* camera) {
    if (camera == nullptr) return;

    // 名前が重複していなければ追加
    if (cameras.find(name) == cameras.end()) {
        cameras.emplace(name, camera);

        // アクティブカメラが未設定なら追加したカメラをアクティブに
        if (!activeCamera) {
            activeCamera = camera;
        }
    }
}

// アクティブカメラ取得
Camera* CameraManager::GetActiveCamera() {
    return activeCamera;
}

// 名前からカメラ取得
Camera* CameraManager::GetCamera(const std::string& name) {
    auto it = cameras.find(name);
    if (it != cameras.end()) {
        return it->second;
    }
    return nullptr;
}

// アクティブカメラ設定（名前で指定）
void CameraManager::SetActiveCamera(const std::string& name) {
    auto it = cameras.find(name);
    if (it != cameras.end()) {
        activeCamera = it->second;
    }
}
