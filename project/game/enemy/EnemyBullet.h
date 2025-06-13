#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"
#include "Math.h"

class EnemyBullet
{
public:
	//初期化
	void Initialize(Object3dBase*object3dBase);
	//更新
	void Update();
	//描画
	void Draw();
	//衝突時コールバック
	void OnCollision();

	//デスフラグが立ったか
	bool IsDead()const { return isDead_; }
public:
///Getter///
//位置
const Vector3& GetPosition()const { return transform_.translate; }
//半径
float GetRadius()const { return radius_; }
//攻撃力
int GetAttack()const { return attack_; }
///Setter///
void SetPosition(const Vector3& position) { transform_.translate = position; }
void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//速度
	Vector3 velocity_;
	//半径
	float radius_ = 0.1f;
	//攻撃力
	uint32_t attack_ = 5;
	//デスフラグ
	bool isDead_ = false;
	//デスタイマー
	int deathTimer_ = kLifeTime;
	//生存時間
	static const int kLifeTime = 60 * 5;
};

