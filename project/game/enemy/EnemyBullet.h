#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"

class EnemyBullet
{
public:
	//初期化
	void Initialize(Object3dBase*object3dBase);
	//更新
	void Update();
	//描画
	void Draw();

private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//速度
	Vector3 velocity_;

	//デスフラグ
	bool isDead_ = false;
};

