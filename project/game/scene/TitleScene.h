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
class TitleScene : public BaseScene
{
public:
	/// <summary>
	/// シーン初期化を行う。
	/// - サウンド／モデルの読み込み、カメラの生成と登録、TitleObject の初期化を行う。
	/// - フェードを初期化して FadeIn を開始し、Skybox とレベルデータ（Json）を読み込む。
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// シーン終了時の後始末を行う。
	/// - CameraManager や Audio 等のサブシステムの Finalize を呼び出してリソースを解放する。
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新を行う。
	/// - カメラ／Skybox／TitleObject を更新し、シーン遷移処理 (SceneChange) とデバッグ処理を呼び出す。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理を行う。
	/// - Object3d の共通描画設定を行った上で Skybox／TitleObject を描画し、Sprite の共通設定とフェードを描画する。
	/// </summary>
	void Draw() override;
	/// <summary>
	/// デバッグ表示を行う（ImGui）。
	/// - カメラ・Skybox のパラメータ編集や TitleObject のデバッグ表示を行う。
	/// </summary>
	void Debug() override;

private:
	/// <summary>
	/// シーン遷移処理を行う。
	/// - フェードの Update を呼び、Space キーで FadeOut を開始する。
	/// - FadeOut 完了時に SceneManager に対して "GAME" シーンへの遷移を要求する。
	/// </summary>
	void SceneChange();

private:
	// パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>> particle_emitters_;
	// カメラ
	std::unique_ptr<Camera> camera_;
	// ライト
	std::unique_ptr<DirectionalLight> directional_light_;
	// フェード
	std::unique_ptr<Fade> fade_;
	// Skybox
	std::unique_ptr<Skybox> skybox_;
	// タイトルオブジェクト
	std::unique_ptr<TitleObject> title_object_;
	// JsonManager
	std::unique_ptr<JsonManager> json_manager_;
	// レベルデータ
	LevelData* level_data_ = nullptr;
};
