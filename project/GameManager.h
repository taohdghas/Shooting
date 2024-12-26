#pragma once
#include <memory>
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"

class GameManager
{
private:

	IScene* iscene_;

	//シーンを保持するメンバ変数
	std::unique_ptr<IScene>sceneArr_[3];

	//現在のシーン
	int currentSceneNo_;
	//前のシーン
	int preSceneNo_;

public:
	GameManager();
	~GameManager();

	//ゲームループ呼び出し
	int Run();
};

