#include "Fade.h"
#include "SpriteBase.h"
#include <algorithm>

//初期化
void Fade::Initialize() {
    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize(SpriteBase::GetInstance(), "resources/white.png");
    sprite_->SetSize({ 1280,720 });
    sprite_->SetPosition({ 0,0 });
    sprite_->SetColor(Vector4{ 0,0,0,1 });
}

//更新
void Fade::Update() {

	switch (state_) {
	case Fade::State::None:
		break;
	case Fade::State::FadeIn:
		count_ += DeltaTime;

		if (count_ >= duration_) {
			count_ = duration_;
		}

		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(1.0f - (count_ / duration_), 0.0f, 1.0f)));

		break;
	case Fade::State::FadeOut:

		count_ += DeltaTime;
	
		if (count_ >= duration_) {
			count_ = duration_;
		}

		sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(count_ / duration_, 0.0f, 1.0f)));

		break;
	default:
		break;
	}

	sprite_ ->Update();

}
//描画
void Fade::Draw() {

	if (state_ == State::None) {return;}

	sprite_->Draw();

}
//フェード開始
void Fade::FadeStart(State status, float duration) {

    state_ = status;
    duration_ = duration;
    count_ = 0.0f;

}
//状態をNoneに
void Fade::End() {

    state_ = State::None;

}
//フェードが終了したか
bool Fade::IsFinished() {
	switch (state_) {
	case Fade::State::FadeIn:
	case Fade::State::FadeOut:
		return count_ >= duration_;
	default:
		return false; 
	}
}

