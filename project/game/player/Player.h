#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Input.h"
#include "Transform.h"
#include "playerBullet.h"
#include "Camera.h"
#include <list>
#include <memory>
#include <vector>

class Player
{
public:
	Player();

	~Player();
	//初期化
	void Initialize(Object3dBase* object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
	//移動
	void Move();
	//攻撃
	void Attack();
	//三方向攻撃
	void ThreeAttack();
	//衝突時コールバック
	void OnCollision();
public:
	///Getter/// 

	//位置取得
	const Vector3& GetPosition()const { return transform_.translate; }
	//弾リスト取得
	const std::list<playerBullet*>& GetBullets()const { return bullets_; }

	///Setter/// 
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	


private:
	Object3dBase* object3dBase_;

	std::unique_ptr<Object3d>object_;
	Camera* camera_;
	Transform transform_;
	//弾のリスト
	std::list<playerBullet*>bullets_;

	//デスフラグ
	bool isDead_ = false;
	//プレイヤーの移動速度
	float speed = 0.1f;
	//プレイヤーの半径
	float radius_ = 1.0f;
	//HP
	uint32_t hp_ = 100;
	int attackCooldown_ = 0;
	static const int attackInterval_ = 10;
};

