#pragma once
#include "Object3d.h"
#include "Input.h"
#include "Transform.h"

class Player
{
public:
	//初期化
	void Initialize(Object3d*object);
	//更新
	void Update();
	//描画
	void Draw();

private:
	Object3d* object_ = nullptr;
};

