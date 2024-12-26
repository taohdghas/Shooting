#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"
#include "Player.h"

class Enemy
{
public:
	//初期化
	void Initialize(Object3dBase*object3dbase, Player* player);
	//更新
	void Update();
	//描画
	void Draw();
	//衝突時コールバック関数
	void OnCollision();

	//発射間隔
	static const int  kFireInterval = 60;
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }
	float GetRadius()const { return radius_; }
private:
	Object3dBase* object3dBase_;
	Transform transform_;
	std::unique_ptr<Object3d>object_;
	Player* player_;
	//速度
	Vector3 velocity_ = {};
	//発射タイマー
	uint32_t fireTimer = 0;
	//追尾を辞める距離
	static const float kApproachDistance;
	//追尾中か
	bool isChasing_ = true;
	//デスフラグ
	bool isDead_ = false;
	//敵の半径
	float radius_ = 1.0f;
};

