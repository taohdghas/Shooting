#pragma once
#include "BaseScene.h"
#include "Fade.h"
#include "GameOverObject.h"
#include "Camera.h"
#include "Skybox.h"

// ゲームオーバーシーン
class GameOverScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// ゲームオーバーシーンの初期化処理を行う。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ゲームオーバーシーンの終了処理を行う
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ゲームオーバーシーンの毎フレーム更新処理を行う。
	/// </summary>
	void Update() override;

	/// <summary>
	/// ゲームオーバーシーンの描画処理を行う。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ゲームオーバーシーンのデバッグ情報を表示する
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
