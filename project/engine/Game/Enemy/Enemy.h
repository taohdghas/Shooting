#pragma once
#include "DirectXBase.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelManager.h"

class Enemy
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
private:
	DirectXBase* directxBase_;
	Object3dBase* object3dBase_;
	Object3d* object3d_;
};

