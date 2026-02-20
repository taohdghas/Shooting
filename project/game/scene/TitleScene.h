#pragma once
#include <vector>
#include <memory>
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Input.h"
#include "BaseScene.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Struct.h"
#include "Fade.h"
#include "Skybox.h"
#include "TitleObject.h"
#include "JsonManager.h"

// タイトルシーン
/// <summary>
/// タイトル画面全体の初期化・更新・描画・終了処理の管理
/// カメラ、スカイボックス、タイトルオブジェクト、フェード、パーティクルエミッター等のリソース管理と連携
/// タイトル画面のUI・演出・操作説明・メニュー選択等の制御
/// シーン遷移処理の実装
/// 毎フレームの状態更新・描画・デバッグ表示の提供
/// </summary>
class TitleScene : public MyEngine::BaseScene
{
public:
	/// <summary>
	/// シーン初期化を行う。
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// シーン終了時の後始末を行う。
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新を行う。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理を行う。
	/// </summary>
	void Draw() override;
	/// <summary>
	/// デバッグ表示を行う（ImGui）
	/// </summary>
	void Debug() override;

private:
	/// <summary>
	/// シーン遷移処理を行う。
	/// </summary>
	void SceneChange();

private:
	// パーティクルエミッター
	std::vector<std::unique_ptr<MyEngine::ParticleEmitter>> particle_emitters_;
	// カメラ
	std::unique_ptr<MyEngine::Camera> camera_;
	// ライト
	std::unique_ptr<DirectionalLight> directional_light_;
	// フェード
	std::unique_ptr<Fade> fade_;
	// Skybox
	std::unique_ptr<Skybox> skybox_;
	// タイトルオブジェクト
	std::unique_ptr<TitleObject> title_object_;
	// JsonManager
	std::unique_ptr<MyEngine::JsonManager> json_manager_;
	// レベルデータ
	std::unique_ptr<LevelData> level_data_;
};
