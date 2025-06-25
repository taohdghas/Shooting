#pragma once
#include "Sprite.h"
#include "Struct.h"

class FadeManager
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//フェードイン
	void FadeIn();
	//フェードアウト
	void FadeOut();
private:
	std::unique_ptr<Sprite>sprite_;

};

