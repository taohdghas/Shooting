#pragma once
#include "BaseScene.h"
#include "Fade.h"
#include "GameOverObject.h"
#include "Camera.h"
#include "Skybox.h"

// ゲームオーバーシーン
/// <summary>
/// ゲームオーバー時のシーン全体の初期化・更新・描画・終了処理の管理
/// カメラ、スカイボックス、ゲームオーバーオブジェクト、フェード等のリソース管理と連携
/// ゲームオーバー演出やUIの制御、シーン遷移処理の実装
/// 毎フレームの状態更新・描画・デバッグ表示の提供
/// </summary>
class GameOverScene : public  MyEngine::BaseScene 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug() override;

public:
	/// <summary>
	/// シーン遷移処理
	/// </summary>
	void SceneChange();

private:
	// カメラ
	std::unique_ptr< MyEngine::Camera> camera_;
	// フェード
	std::unique_ptr<Fade> fade_;
	// ゲームオーバーオブジェクト
	std::unique_ptr<GameOverObject> game_over_object_;
	// Skybox
	std::unique_ptr<Skybox> skybox_;
	// タイトルシーンへの移行フラグ
	bool is_to_title_ = false;
	// ゲームシーンへの移行フラグ
	bool is_to_game_ = false;
};