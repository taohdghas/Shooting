#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"

//ゲームオーバーオブジェクト
class GameOverObject
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//デバック
	void Debug();
private:
	//ゲームオーアー字のオブジェクト
	std::unique_ptr<Object3d>gameover;
	//retryのオブジェクト
	std::unique_ptr<Object3d>retry;
};

