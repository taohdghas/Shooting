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

//タイトルシーン
class TitleScene : public BaseScene
{
public:
	//初期化
	void Initialize() override;
	//終了
	void Finalize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//デバック
	void Debug()override;
	//シーン遷移
	void SceneChange();
private:
	//パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>>particleEmitter;
	//カメラ
	std::unique_ptr<Camera>camera;
	//ライト
	std::unique_ptr<DirectionalLight>directionallight;
	//フェード
	std::unique_ptr<Fade>fade;
	//Skybox
	std::unique_ptr<Skybox>skybox;
	//タイトルオブジェクト
	std::unique_ptr<TitleObject>titleObject;
	//JsonManager
	std::unique_ptr<JsonManager>jsonManager;
	//レベルデータ
	LevelData* levelData;
};

