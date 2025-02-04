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
	//Sprite初期化
	std::vector<std::unique_ptr<Sprite>>sprites;
	//3Dオブジェクト
	std::vector<std::unique_ptr<Object3d>>object3ds;
};

