#pragma once
#include <vector>
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"

class GameScene
{
public:
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	//Sprite初期化
	std::vector<Sprite*>sprites;
	//3Dオブジェクト
	std::vector<Object3d*>object3ds;
};

