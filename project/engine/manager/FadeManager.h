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
	void FadeIn(float duration);
	//フェードアウト
	void FadeOut(float duration);
public:
	///Getter///


	///Setter///

private:
	std::unique_ptr<Sprite>sprite_;
	bool faseIn_ = false;
	bool faseOut_ = false;
	float faseTimer_ = 0.0f;
	float faseDuration_ = 1.0f;
};

