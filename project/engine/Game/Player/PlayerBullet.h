#pragma once
#include "DirectXBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Transform.h"
#include "Vector3.h"
#include "Math.h"
class PlayerBullet
{
public:
	void Initialize();

	void Update();

	void Draw();
    //デスフラグが立ったか
	bool IsDead()const { return isDead_; }
public:
	///Getter///
	const Vector3& GetTranlate()const { return transform_.translate; }
private:
	Transform transform_;
	Model* model_;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer;
	//デスフラグ
	bool isDead_ = false;
};

