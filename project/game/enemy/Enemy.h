#pragma once
#include "EnemyBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"

class Enemy : public EnemyBase
{
public:
	//初期化
	void Initialize(Object3dBase*object3dBase)override;
	//更新 
	void Update() override;
	//描画
	void Draw()override;
	//攻撃(レーザー)
	void Laser();
	//衝突時コールバック
	void onCollision();
	//HP減少
	void TakeDamage(int damege)override;
	//Debug
	void Debug(int id);
	//デスフラグが立ったか
	bool IsDead()const override{ return isDead_; }
	//デスパーティクル発生フラグが立ったか
	bool IsDeathParticle()const { return isDeathParticle_; }

public:
	///Gettter///
	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()const { return bullets_; }

	///Setter///
	//デスパーティクルフラグ
	void SetisDeathParticle(bool flag) { isDeathParticle_ = flag; }
private:
	//弾のリスト
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	//移動速度
	Vector3 velocity_ = { 0.0,0 };
	//デスパーティクル発生フラグ
	bool isDeathParticle_ = false;
	//レーザー発射カウントタイマー
	int fireTimerCount_ = 0;
	//発射間隔
	static const int kFireInterval = 30;
	//発射距離
	const float fireDistance = 25.0f;
	//攻撃停止距離Z
	const float attackStopDisntanceZ = 6;
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;
};

