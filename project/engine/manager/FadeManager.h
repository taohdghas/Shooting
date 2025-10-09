#pragma once
/*
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
	//フェード中か
	bool IsFade()const { return fadeIn_ || fadeOut_; }
	//フェードイン終了
	bool IsFadeInEnd()const { return fadeInEnd_; }
	//フェードアウト終了
	bool IsFadeOutEnd() const { return fadeOutEnd_; }
public:
	///Getter///


	///Setter///

private:
	std::unique_ptr<Sprite>sprite_;
	bool fadeIn_ = false;
	bool fadeOut_ = false;
	bool fadeStarted_ = false;
	bool fadeInEnd_ = false;
	bool fadeOutEnd_ = false;
	float fadeTimer_ = 0.0f;
	float fadeDuration_ = 1.0f;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
};

*/