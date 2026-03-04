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
	/// 初期化
	/// <returns>なし</returns>
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// <returns>なし</returns>
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードを開始
	/// <param name="state">開始するフェードの状態</param>
	/// <param name="duration">フェードの持続時間（秒）</param>
	/// <returns>なし</returns>
	/// </summary>
	void FadeStart(State state, float duration);

	/// <summary>
	/// フェード状態を停止しにする。
	/// <returns>なし</returns>
	/// </summary>
	void End();

	/// <summary>
	/// フェードが終了したか判定
	/// <returns>フェードが終了していればtrue</returns>
	/// </summary>
	bool IsFinished();

public:
	/// <summary>
	/// 現在のフェード状態を取得
	/// <returns>現在のフェード状態</returns>
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
