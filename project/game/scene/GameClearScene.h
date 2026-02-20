#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "Skybox.h"
#include "GameClearObject.h"
#include "Fade.h"

// ゲームクリアシーン
/// <summary>
/// ゲームクリア時のシーン全体の初期化・更新・描画・終了処理の管理
/// カメラ、スカイボックス、ゲームクリアオブジェクト、フェード等のリソース管理と連携
/// ゲームクリア演出やUIの制御、シーン遷移処理の実装
/// 毎フレームの状態更新・描画・デバッグ表示の提供
/// </summary>
class GameClearScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// 初期化処理を行う。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理を行う
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理を行う。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示用の処理を行う。
	/// </summary>
	void Debug() override;
public:
	/// <summary>
    /// シーン遷移処理を行う。
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

