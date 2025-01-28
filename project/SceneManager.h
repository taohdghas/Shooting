#pragma once
#include "BaseScene.h"

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
	//次のシーンをセット
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
private:
	static SceneManager* instance;
	SceneManager* sceneManager_ = nullptr;
	//現在のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;
};

