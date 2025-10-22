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
	/// 初期化する。
	/// 内部でスプライトを生成し、"resources/white.png" を読み込む。
	/// サイズは 1280x720、位置は (0,0)、初期色は黒でアルファ 1.0（不透明）。
	/// </summary>
	void Initialize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - state_ が <c>State::None</c> の場合は何もしない。
	/// - <c>count_</c> を <c>DeltaTime</c> だけ進め、<c>duration_</c> を上限とする。
	/// - 正規化した t = clamp(count_ / duration_, 0, 1) に対して easeOutQuad を適用してイージングを取得し、
	///   フェード種別に応じてアルファ値を計算してスプライトに反映する。
	/// - 最後にスプライトの <c>Update()</c> を呼ぶ。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - state_ が <c>State::None</c> の場合は描画を行わない。
	/// - それ以外では内部スプライトの <c>Draw()</c> を呼ぶ。
	/// </summary>
	void Draw();
	/// <summary>
	/// フェードを開始する。
	/// </summary>
	/// <param name="state">開始するフェードの種類（<c>State::FadeIn</c> または <c>State::FadeOut</c>）。</param>
	/// <param name="duration">フェードの持続時間（秒）。</param>
	/// <remarks>state_ に指定をセットし、duration_ を設定、count_ を 0 にリセットする。</remarks>
	void FadeStart(State state, float duration);
	/// <summary>
	/// フェード状態を <c>State::None</c> に設定して停止する。
	/// </summary>
	void End();
	/// <summary>
	/// フェードが終了したかを判定する。
	/// </summary>
	/// <returns>
	/// フェード中（<c>FadeIn</c> / <c>FadeOut</c>）でかつ <c>count_ >= duration_</c> の場合は true、それ以外は false。
	/// </returns>
	bool IsFinished();
public:
	/// <summary>現在のフェード状態を取得する。</summary>
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
