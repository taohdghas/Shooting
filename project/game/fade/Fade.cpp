#include "Fade.h"
#include "SpriteBase.h"
#include <algorithm>

double easeOutQuad(double x) {
    return 1.0 - (1.0 - x) * (1.0 - x);
}
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
    if (state_ == State::None) {
        return;
    }

    //経過時間を進める
    count_ += DeltaTime;
    if (count_ > duration_) {
        count_ = duration_;
    }

    float t = std::clamp(count_ / duration_, 0.0f, 1.0f);

    //イージングを適用
    float eased = float(easeOutQuad(t));

    float alpha = 1.0f;

    switch (state_) {
    case State::FadeIn:
        alpha = 1.0f - eased;
        break;

    case State::FadeOut:
        alpha = eased;
        break;

    default:
        break;
    }

    sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, alpha));
    sprite_->Update();
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

