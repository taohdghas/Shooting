#pragma once
#include "Sprite.h"
#include "Struct.h"

//フェードを管理するクラス
class Fade
{
public:
	//フェードの状態
	enum class State {
		None,
		FadeIn,
		FadeOut,
	};

	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//フェード開始
	void FadeStart(State state, float duration);
	//状態をNoneに
	void End();
	//フェード終了
	bool IsFinished();
public:
	State GetState() const { return state_; }
private:
	std::unique_ptr<Sprite> sprite_;
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;

	//初期状態
	State state_ = State::None;

	//フェードの持続時間
	float duration_ = 0.0f;
	//経過時間
	float count_ = 0.0f;
};
