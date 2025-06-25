#include "SceneManager.h"
#include <cassert>

SceneManager* SceneManager::instance = nullptr;

//シングルトンインスタンス
SceneManager* SceneManager::GetInstance() {
	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

//終了
void SceneManager::Finalize() {
	//最後のシーンの終了と解放
	scene_->Finalize();
	delete instance;
	instance = nullptr;
}

//更新
void SceneManager::Update() {
	//次のシーンがセットされているなら
	if (nextScene_) {
		//前のシーンの終了
		if (scene_) {
			scene_->Finalize();
		}
		//シーン切り替え
		scene_ = std::move(nextScene_);
		nextScene_ = nullptr;
		//シーンマネージャをセット
		scene_->SetSceneManager(this);
		//次のシーンを初期化
		scene_->Initialize();
	}

	//実行中のシーンの更新
	scene_->Update();
}

//描画
void SceneManager::Draw() {
	scene_->Draw();
}

//次のシーンを予約
void SceneManager::ChangeScene(const std::string& sceneName) {
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//次のシーンを生成
	nextScene_ = sceneFactory_->CreateScene(sceneName);
}
