#pragma once
#include "BaseScene.h"

//ゲームオーバーシーン
class GameOverScene : public BaseScene
{
public:
	/// <summary>
	/// シーン初期化処理。
	/// - シーン開始時に必要なリソース読み込みや状態リセットを行う。
	/// - 例: フェードやUI、サウンドの再生準備などをここで行う想定。
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// シーン終了処理。
	/// - シーン終了時に確保したリソースの解放や状態のクリーンアップを行う。
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// 毎フレーム更新処理。
	/// - ユーザー入力（リトライやタイトルへ戻る等）やアニメーション、フェード状態の更新を行う。
	/// - 必要に応じて SceneManager によるシーン遷移要求を行う。
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画処理。
	/// - 画面上の UI や背景、フェード等の描画コマンドを発行する。
	/// </summary>
	void Draw()override;
	/// <summary>
	/// デバッグ表示処理（ImGui 等）。
	/// - デバッグ用 UI の表示やパラメータ調整を行う（実装はオプション）。
	/// </summary>
	void Debug()override;
private:

};

