#include "Fade.h"
#include "SpriteBase.h"
#include "MyMath.h"
#include <algorithm>
#include <unordered_map>
#include <functional>

// フェード種別ごとのアルファ計算関数
using AlphaFunc = std::function<float(float)>;

static const std::unordered_map<Fade::State, AlphaFunc> kAlphaFuncTable = {
    { Fade::State::FadeIn,  [](float eased) { return 1.0f - eased; } },
    { Fade::State::FadeOut, [](float eased) { return eased; } }
};

// フェード用スプライトの初期化
void Fade::Initialize() {
    sprite_ = std::make_unique< MyEngine::Sprite>();
    sprite_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/white.png");
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
    float eased = static_cast<float>(Math::easeOutQuad(t));

    float alpha = 1.0f;

    // データドリブンでアルファ値を決定
    auto it = kAlphaFuncTable.find(state_);
    if (it != kAlphaFuncTable.end()) {
        alpha = it->second(eased);
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