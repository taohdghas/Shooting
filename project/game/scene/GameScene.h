#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "BaseScene.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "JsonManager.h"

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Skybox.h"

class GameScene : public BaseScene
{
public:
	//初期化
	void Initialize()override;
	//終了
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
	//デバック
	void Debug()override;

private:
	//プレイヤー
	std::unique_ptr<Player>player;
	//敵
	std::unique_ptr<Enemy>enemy;
	//Skybox
	std::unique_ptr<Skybox>skybox;
	//パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>>particleEmitter;
	//衝突マネージャー
	std::unique_ptr<CollisionManager>collisionManager;
	//カメラ
	std::unique_ptr<Camera>camera;
};

