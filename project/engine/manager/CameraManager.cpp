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

}

//終了
void CameraManager::Finalize() {
	delete instance;
	instance = nullptr;
}