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
	void Initialize(Object3dBase* object3dbase);
	void Update();
	void Draw();
	void OnCollision();

	bool IsDead()const { return isDead_; }
public:
	const Vector3& GetPosition()const { return transform_.translate; }
	float GetRadius()const { return radius_; }
	int GetAttack()const { return attack; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private:
	std::unique_ptr<Object3d>object_;
	Object3dBase* object3dbase_;
	Transform transform_;
	Vector3 velocity_;
	//寿命
	static const uint32_t kLifeTime = 60 * 5;
	//デスタイマー
	int deathTimer;
	//デスフラグ
	bool isDead_ = false;
	float radius_ = 1.0f;
	uint32_t attack = 5;
};

