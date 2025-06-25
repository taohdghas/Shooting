#include "FadeManager.h"
#include "SpriteBase.h"

//初期化
void FadeManager::Initialize() {
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteBase::GetInstance(), "resources/white.png");
	sprite_->SetSize({ 1280,720 });
	sprite_->SetPosition({ 0,0 });
	sprite_->SetColor({ 0,0,0,0 });
}

//更新
void FadeManager::Update() {

	sprite_->Update();
}

//描画
void FadeManager::Draw() {

	sprite_->Draw();
}

//フェードイン
void FadeManager::FadeIn(float duration) {

}

//フェードアウト
void FadeManager::FadeOut(float duration) {

}