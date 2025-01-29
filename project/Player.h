#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Input.h"
#include "Transform.h"
#include "Camera.h"
#include "PlayerBullet.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(Object3dBase*object3dbase);
	void Update();
	void Draw();
	void Attack();
	void OnCollision();
	void TakeDamage(int damage);
	bool IsDead()const { return isDead_; }
public:
	const Vector3& GetPosition()const { return transform_.translate; }
	float GetRadius()const { return radius_; }
	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; }
	std::vector<float>GetBulletRadius()const {
		std::vector<float>radius;
		for (const auto& bullet : bullets_) {
			radius.push_back(bullet->GetRadius());
		}
		return radius;
	}
	int GetHP()const { return hp_; }
private:
	Object3dBase* object3dBase_;
	Camera* camera_;
	Transform transform_;
	std::unique_ptr<Object3d>object_;
	std::list<PlayerBullet*>bullets_;
	float speed = 0.1f;
	float radius_ = 1.0f;//デスフラグ
	bool isDead_ = false;
	uint32_t hp_ = 100;
};

