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
	/// - カメラ、フェード、ゲームオーバーオブジェクト、Skyboxなどの初期化を実施する。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ゲームオーバーシーンの終了処理を行う。
	/// - 使用しているリソースの解放や後処理を実施する。
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// ゲームオーバーシーンの毎フレーム更新処理を行う。
	/// - 入力判定やオブジェクトの状態更新、シーン遷移判定などを実施する。
	/// </summary>
	void Update() override;

	/// <summary>
	/// ゲームオーバーシーンの描画処理を行う。
	/// - カメラ、Skybox、ゲームオーバーオブジェクト、フェードなどの描画を実施する。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// ゲームオーバーシーンのデバッグ情報を表示する。
	/// - オブジェクトの状態や内部情報の可視化を行う。
	/// </summary>
	void Debug() override;

public:
	/// <summary>
	/// シーン遷移処理を行う。
	/// - タイトルシーンやゲームシーンへの移行判定・実行を行う。
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
