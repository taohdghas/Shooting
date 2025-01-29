#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

class SceneManager
{
public:
	//シングルトンインスタンス
	static SceneManager* GetInstance();
	//デストラクタ
	//~SceneManager();
	//終了
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw();
public:
	//次のシーンを予約
	void ChangeScene(const std::string& sceneName);
	//シーンファクトリーのsetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	static SceneManager* instance;
	SceneManager* sceneManager_ = nullptr;
	//現在のシーン
	std::unique_ptr<BaseScene>scene_;
	//BaseScene* scene_ = nullptr;
	//次のシーン
	std::unique_ptr<BaseScene>nextScene_;
	//BaseScene* nextScene_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

