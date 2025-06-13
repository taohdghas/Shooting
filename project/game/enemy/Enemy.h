#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"

class Player;
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
	//衝突時コールバック
	void onCollision();
	//HP減少
	void TakeDamage(int damege);
	//デスフラグが立ったか
	bool IsDead()const { return isDead_; }
public:
	///Gettter///
	//位置
	const Vector3& GetPosition()const { return transform_.translate; }
	//半径
	float GetRadius()const { return radius_; }
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()const { return bullets_; }

private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//プレイヤー
	Player*player_;
	//弾のリスト
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	//移動速度
	Vector3 velocity_ = { 0.0,0 };
	//HP
	uint32_t hp_ = 100;
	//半径
	float radius_ = 1.0f;
	//デスフラグ
	bool isDead_ = false;
	//レーザー発射カウントタイマー
	int fireTimerCount_ = 0;
	//発射間隔
	static const int kFireInterval = 30;
	//発射タイマー
	int fireTimer_ = 0;
};

