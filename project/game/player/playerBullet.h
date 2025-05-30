#pragma once
#include "Object3d.h"
#include "Transform.h"
#include <memory>

class playerBullet
{
public:
	//初期化
	void Initialize(std::unique_ptr<Object3d>object);
	//更新
	void Update();
	//描画
	void Draw();
	//デスフラグがたったか
	bool IsDead()const { return isDead_; }
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }

	///Setter///
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private:
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//速度
	Vector3 velocity_;

	//デスフラグ
	bool isDead_ = false;
};

