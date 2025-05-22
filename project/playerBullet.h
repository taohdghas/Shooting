#pragma once
#include "Object3d.h"
#include "Transform.h"

class playerBullet
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
	Transform transform_;

	//速度
	Vector3 velocity_;
};

