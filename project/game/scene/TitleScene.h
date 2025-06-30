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
#include "FadeManager.h"

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
private:
	//パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>>particleEmitter;
	//カメラ
	std::unique_ptr<Camera>camera;
	//ライト
	std::unique_ptr<DirectionalLight>directionallight;
	//タイトルのオブジェクト
	std::unique_ptr<Object3d>title;
	//pushspaceのオブジェクト
	std::unique_ptr<Object3d>pushspace;
	//フェードマネージャー
	std::unique_ptr<FadeManager>fadeManager;

	//スタート演出
	bool pushspaceMove = false;
	bool fadeOutStarted = false;
};

