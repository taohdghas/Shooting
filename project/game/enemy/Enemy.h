#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"

class Enemy
{
public:
	//初期化
	void Initialize(Object3dBase*object3dBase);
	//更新
	void Update();
	//描画
	void Draw();
	//攻撃(レーザー)
	void Laser();
private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;

	//移動速度
	Vector3 velocity_ = { 0.0,0 };

	//デスフラグ
	bool isDead_ = false;
};

