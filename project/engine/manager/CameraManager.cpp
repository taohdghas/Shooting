#include "CameraManager.h"

CameraManager* CameraManager::instance_ = nullptr;

// シングルトンインスタンス取得
CameraManager* CameraManager::GetInstance() {
    if (instance_ == nullptr) {
        instance_ = new CameraManager;
    }
    return instance_;
}

// 初期化
void CameraManager::Initialize() {
    cameras_.clear();        // 登録済みカメラをクリア
    active_camera_ = nullptr; // アクティブカメラをリセット
}

// 終了
void CameraManager::Finalize() {
    cameras_.clear();        // 登録済みカメラをクリア
    delete instance_;        // シングルトン解放
    instance_ = nullptr;
}

// カメラを追加
void CameraManager::AddCamera(const std::string& name, Camera* camera) {
    if (camera == nullptr) return;

    // 名前が重複していなければ追加
    if (cameras_.find(name) == cameras_.end()) {
        cameras_.emplace(name, camera);

        // アクティブカメラが未設定なら追加したカメラをアクティブに
        if (!active_camera_) {
            active_camera_ = camera;
        }
    }
}

// アクティブカメラ取得
Camera* CameraManager::GetActiveCamera() {
    return active_camera_;
}

// 名前からカメラ取得
Camera* CameraManager::GetCamera(const std::string& name) {
    auto it = cameras_.find(name);
    if (it != cameras_.end()) {
        return it->second;
    }
    return nullptr;
}

// アクティブカメラ設定
void CameraManager::SetActiveCamera(const std::string& name) {
    auto it = cameras_.find(name);
    if (it != cameras_.end()) {
        active_camera_ = it->second;
    }
}
