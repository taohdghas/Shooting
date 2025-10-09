/*
#include "FadeManager.h"
#include "SpriteBase.h"
#include <algorithm>

//初期化
void FadeManager::Initialize() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteBase::GetInstance(), "resources/white.png");
	sprite_->SetSize({ 1280,720 });
	sprite_->SetPosition({ 0,0 });
	sprite_->SetColor(Vector4{ 0,0,0,0 });
}

//更新
void FadeManager::Update() {
	//flag立ってないならスキップ
	if (!fadeIn_ && !fadeOut_) {
		return;
	}

	fadeTimer_ += kDeltaTime;
	float t = std::clamp(fadeTimer_ / fadeDuration_, 0.0f, 1.0f);

	float alpha = fadeIn_ ? (1.0f - t) : t;
	sprite_->SetColor(Vector4{ 0,0,0,alpha });

	if (t >= 1.0f) {
		if (fadeOut_) {
			fadeOut_ = false;
			fadeOutEnd_ = true;
		}
		if (fadeIn_) {
			fadeIn_ = false;
			fadeInEnd_ = true;
		}
	}

	sprite_->Update();
}

//描画
void FadeManager::Draw() {

	sprite_->Draw();
}

//フェードイン
void FadeManager::FadeIn(float duration) {
	fadeIn_ = true;
	fadeOut_ = false;
	fadeTimer_ = 0.0f;
	fadeDuration_ = duration;
	sprite_->SetColor(Vector4{ 0,0,0,1 });
}

//フェードアウト
void FadeManager::FadeOut(float duration) {
	fadeIn_ = false;
	fadeOut_ = true;
	fadeTimer_ = 0.0f;
	fadeDuration_ = duration;
	sprite_->SetColor(Vector4{ 0,0,0,0 });
}
*/