#include "Fade.h"
#include "SpriteBase.h"
#include "MyMath.h"
#include <algorithm>

// フェード用スプライトの初期化
void Fade::Initialize() {
    sprite_ = std::make_unique<Sprite>();
    sprite_->Initialize(SpriteBase::GetInstance(), "resources/white.png");
    sprite_->SetSize({ 1280, 720 });
    sprite_->SetPosition({ 0, 0 });
    sprite_->SetColor(Vector4{ 0, 0, 0, 1 });
}

// 毎フレームの更新処理
void Fade::Update() {
    if (state_ == State::None) {
        return;
    }

    // 経過時間を進める（duration_ を上限としてクリップ）
    count_ += kDeltaTime;
    if (count_ > duration_) {
        count_ = duration_;
    }

    // 正規化した進行度 t を算出
    float t = std::clamp(count_ / duration_, 0.0f, 1.0f);

    // イージング適用
    float eased = static_cast<float>(easeOutQuad(t));

    float alpha = 1.0f;

    // フェード種別に応じてアルファ値を決定
    switch (state_) {
    case State::FadeIn:
        alpha = 1.0f - eased;  // フェードイン：徐々に透明に
        break;

    case State::FadeOut:
        alpha = eased;         // フェードアウト：徐々に不透明に
        break;

    default:
        break;
    }

    // スプライトの色（アルファ）を更新
    sprite_->SetColor(Vector4(0.0f, 0.0f, 0.0f, alpha));
    sprite_->Update();
}

// フェードスプライトの描画
void Fade::Draw() {
    if (state_ == State::None) {
        return;
    }
    sprite_->Draw();
}

// フェード開始処理
void Fade::FadeStart(State state, float duration) {
    state_ = state;
    duration_ = duration;
    count_ = 0.0f;
}

// フェード状態を None にする（停止）
void Fade::End() {
    state_ = State::None;
}

// フェードが終了したか判定
bool Fade::IsFinished() {
    switch (state_) {
    case State::FadeIn:
    case State::FadeOut:
        return count_ >= duration_;
    default:
        return false;
    }
}
