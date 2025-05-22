#pragma once
#include "Object3d.h"
#include "Input.h"
#include "Transform.h"
#include "playerBullet.h"
#include "Camera.h"

class Player
{
public:
	//初期化
	void Initialize(Object3d*object);
	//更新
	void Update();
	//描画
	void Draw();
	//移動
	void Move();
	//攻撃
	void Attack();
private:
	Object3d* object_ = nullptr;
	Camera* camera_;
	Transform transform_;

	//移動速度
	float speed = 1.0f;
};

