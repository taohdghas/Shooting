#pragma once
#include <vector>
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "BaseScene.h"
#include "Camera.h"

#include "Player.h"
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
	//天球
	std::unique_ptr<Skydome>skydome;
	Object3d* skydomeObject;
};

