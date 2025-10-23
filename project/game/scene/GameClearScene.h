#pragma once
#include "BaseScene.h"

// ゲームクリアシーン
class GameClearScene : public BaseScene
{
public:
	/// <summary>
	/// 初期化処理を行う。
	/// - シーン開始時のリソース確保や状態リセットを行う場所（現状は空実装）。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理を行う。
	/// - シーン終了時のクリーンアップを行う場所（現状は空実装）。
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - Enter (DIK_RETURN) が押されたらシーンマネージャに対して "TITLE" シーンへ切り替えを要求する。
	/// - その他のゲーム処理（入力やアニメーション等）があればここで扱う。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理を行う。
	/// - シーンの描画コマンドを発行する場所（現状は空実装）。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示用の処理を行う。
	/// - ImGui 等によるデバッグ UI を表示する場合はここに実装する（現状は空実装）。
	/// </summary>
	void Debug() override;

private:
};

