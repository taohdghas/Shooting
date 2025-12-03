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

	/// <summary>
	/// フェード機能を初期化する。
	/// - スプライトのサイズや初期位置、初期色などを設定する。
	/// </summary>
	void Initialize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - state_ が <c>State::None</c> の場合は何もしない。
	/// - 経過時間 <c>count_</c> を進め、duration_ を上限としてクリップする。
	/// - 正規化した t = clamp(count_ / duration_, 0, 1) に対してイージングを適用し、フェード種別に応じてスプライトのアルファを更新する。
	/// - 最後に内部スプライトの <c>Update()</c> を呼ぶ。
	/// </summary>
	void Update();
	/// <summary>
	/// フェードスプライトを描画する。
	/// - state_ が <c>State::None</c> の場合は描画を行わない。
	/// - それ以外では内部スプライトの <c>Draw()</c> を呼ぶ。
	/// </summary>
	void Draw();
	/// <summary>
	/// フェードを開始する。
	/// </summary>
	/// <param name="state">開始するフェードの種類（<c>State::FadeIn</c> または <c>State::FadeOut</c>）。</param>
	/// <param name="duration">フェードにかける時間（秒）。</param>
	/// <remarks>
	/// - 内部で <c>state_</c> に指定値を設定し、<c>duration_</c> を設定、<c>count_</c> を 0 にリセットする。
	/// </remarks>
	void FadeStart(State state, float duration);
	/// <summary>
	/// フェード状態を停止し <c>State::None</c> にする。
	/// </summary>
	void End();
	/// <summary>
	/// フェードが終了したか判定する。
	/// - <c>State::FadeIn</c> / <c>State::FadeOut</c> のいずれかの状態でかつ <c>count_ >= duration_</c> の場合 true を返す。
	/// </summary>
	/// <returns>フェード完了なら true、そうでなければ false。</returns>
	bool IsFinished();
public:
	/// <summary>
	/// 現在のフェード状態を取得する。
	/// </summary>
	/// <returns>現在の <c>State</c>。</returns>
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
