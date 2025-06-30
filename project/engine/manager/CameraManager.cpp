#include "CameraManager.h"

CameraManager* CameraManager::instance = nullptr;

//シングルトンインスタンス
CameraManager* CameraManager::GetInstance() {
	if (instance == nullptr) {
		instance = new CameraManager;
	}
	return instance;
}

//初期化
void CameraManager::Initialize() {
	cameras.clear();
	activeCamera = nullptr;
}

//終了
void CameraManager::Finalize() {
	cameras.clear();
	delete instance;
	instance = nullptr;
}

//カメラ追加
void CameraManager::AddCamera(const std::string& name,Camera*camera) {
	if (camera == nullptr) return;
	if (cameras.find(name) == cameras.end()) {
		cameras.emplace(name, camera);
		if (!activeCamera) {
			activeCamera = camera;
		}
	}
}

//アクティブカメラ取得
Camera* CameraManager::GetActiveCamera() {
	return activeCamera;
}

//名前からカメラを取得
Camera* CameraManager::GetCamera(const std::string& name) {
	auto it = cameras.find(name);
	if (it != cameras.end()) {
		return it->second;
	}
	return nullptr;
}

//アクティブカメラ設定
void CameraManager::SetActiveCamera(const std::string& name) {
	auto it = cameras.find(name);
	if (it != cameras.end()) {
		activeCamera = it->second;
	}
}
