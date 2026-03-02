#pragma once
#include "Sprite.h"
#include "Struct.h"

//フェードを管理するクラス
/// <summary>
/// 画面のフェードイン／フェードアウト演出の状態管理と制御
/// フェードの開始・終了・進行状況の管理
/// スプライトを用いたフェード描画処理の実装
/// 毎フレームの更新処理によるアルファ値の補間・制御
/// 外部からのフェード状態取得やフェード完了判定の提供
/// </summary>
class Fade
{
public:
	//フェードの状態
	enum class State {
		None,
		FadeIn,
		FadeOut,
	};

	/// <summary>
	/// フェード機能を初期化する
	/// </summary>
	void Initialize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update();
	/// <summary>
	/// フェードスプライトを描画する。
	/// </summary>
	void Draw();
	/// <summary>
	/// フェードを開始する。
	/// </summary>
	void FadeStart(State state, float duration);
	/// <summary>
	/// フェード状態を停止し <c>State::None</c> にする。
	/// </summary>
	void End();
	/// <summary>
	/// フェードが終了したか判定する。
	/// </summary>
	bool IsFinished();
public:
	/// <summary>
	/// 現在のフェード状態を取得する。
	/// </summary>
	State GetState() const { return state_; }

private:
	std::unique_ptr< MyEngine::Sprite> sprite_;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//初期状態
	State state_ = State::None;
	//フェードの持続時間
	float duration_ = 0.0f;
	//経過時間
	float count_ = 0.0f;
};