#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameClearObject.h"
#include "Fade.h"

// ゲームクリアシーン
class GameClearScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// 初期化処理を行う。
	/// - シーン開始時のリソース確保や状態リセットを行う場所
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理を行う。
	/// - シーン終了時のクリーンアップを行う場所
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
	/// - シーンの描画コマンドを発行する場所
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示用の処理を行う。
	/// - ImGui 等によるデバッグ UI を表示する場合はここに実装する
	/// </summary>
	void Debug() override;
public:
	/// <summary>
    /// シーン遷移処理を行う。
    /// - ゲームクリア後に次のシーン（例: タイトル画面など）へ切り替えるための処理を実装する。
    /// - フェード演出やシーンマネージャへのシーン変更要求などを行う場合はこの関数内で記述する。
    /// </summary>
	void SceneChange();
private:
	// カメラ
	std::unique_ptr< MyEngine::Camera> camera_;
	// Skybox
	std::unique_ptr<Skybox> skybox_;
	// ゲームクリアオブジェクト
	std::unique_ptr<GameClearObject> game_clear_object_;
	//フェード
	std::unique_ptr<Fade>fade_;
};

