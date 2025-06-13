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

#include <vector>

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"

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
private:
	//プレイヤー
	std::unique_ptr<Player>player;
	Object3d* playerObject;
	//敵
	std::unique_ptr<Enemy>enemy;

	//天球
	std::unique_ptr<Skydome>skydome;
	Object3d* skydomeObject;

	//パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>>particleEmitter;
	//衝突マネージャー
	std::unique_ptr<CollisionManager>collisionManager;
};

