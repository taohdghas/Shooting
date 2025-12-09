#pragma once
#include "BaseScene.h"

//ゲームセレクトシーン
class GameSelectScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// シーンの初期化処理を行う。
	/// - シーン開始時に必要なリソースの生成・読み込みや状態の初期化を行う場所。
	/// - 現在の実装では空実装（将来的にモデル読み込みやカメラ設定などを追加する想定）。
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// シーンの終了処理を行う。
	/// - シーン終了時に確保したリソースの解放や状態のクリーンアップを行う場所。
	/// - 現在の実装では空実装。
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 入力処理や UI 更新、シーン内オブジェクトの更新をここで行う。
	/// - 現在の実装では空実装。
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画処理を行う。
	/// - シーン内の 3D/2D オブジェクトの描画呼び出しを行う場所。
	/// - 現在の実装では空実装。
	/// </summary>
	void Draw()override;
	/// <summary>
	/// デバッグ用の処理を行う。
	/// - ImGui 等を使ったデバッグ表示やパラメータ調整を行う場所。
	/// - 現在の実装では空実装。
	/// </summary>
	void Debug()override;
private:

};

