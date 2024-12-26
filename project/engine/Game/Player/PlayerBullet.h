#pragma once
#include "DirectXBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "Vector3.h"
#include "Math.h"
class PlayerBullet
{
public:
	void Initialize(Object3dBase*object3dbase);

	void Update();

	void Draw();
	//衝突時コールバック関数
	void OnCollision();

    //デスフラグが立ったか
	bool IsDead()const { return isDead_; }
public:
	///Getter///
	const Vector3& GetTranlate()const { return transform_.translate; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
private:
	std::unique_ptr<Object3d>object_;
	Object3dBase* object3dbase_;
	Transform transform_;
	//速度
	Vector3 velocity_;
	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer;
	//デスフラグ
	bool isDead_ = false;
};

