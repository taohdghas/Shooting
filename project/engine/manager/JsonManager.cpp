#include "JsonManager.h"

JsonManager* JsonManager::instance = nullptr;

//シングルトンインスタンスの取得
JsonManager* JsonManager::GetInstance() {
	if (instance == nullptr) {
		instance = new JsonManager;
	}
	return instance;
}