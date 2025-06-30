#pragma once
#include "Sprite.h"
#include "SpriteBase.h"

class Ui
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();

private:
	std::unique_ptr<Sprite>hpBar_;

};

