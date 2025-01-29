#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"
#include "Camera.h"
#include "enemybullet.h"

enum class Phase {
	Laser,
	Homing
};

class Player;
class Enemy
{
public:
	Enemy();
	~Enemy();
	//初期化
	void Initialize(Object3dBase* object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
	//レーザーフェーズ
	void Laser();
	//追尾弾フェーズ
	void Homing();
	//衝突時コールバック関数
	void OnCollision();
	//HP減少関数
	void TakeDamage(int damage);
	//デスフラグが立ったか
	bool IsDead()const { return isDead_; }
	//発射間隔
	static const int  kFireInterval = 30;
public:
	const Vector3& GetPosition()const { return transform_.translate; }
	float GetRadius()const { return radius_; }
	int GetHP()const { return hp_; }
	std::list<enemybullet*>GetBullets()const {
		std::list<enemybullet*>bullets;
		for (const auto& bullet : bullets_) {
			bullets.push_back(bullet.get());
		}
		return bullets;
	}
	void SetPlayer(Player* player) { player_ = player; }
	void SetPosition(const Vector3& position) { transform_.translate; }
private:
	Object3dBase* object3dBase_;
	Camera* camera_;
	Transform transform_;
	Player* player_;
	std::unique_ptr<Object3d>object_;
	//弾のリスト
	std::list<std::unique_ptr<enemybullet>> bullets_;
	//速度
	Vector3 velocity_ = { 0,0,0 };
	//追尾を辞める距離
	static const float kApproachDistance;
	//追尾中か
	bool isChasing_ = true;
	//デスフラグ
	bool isDead_ = false;
	//敵の半径
	float radius_ = 1.0f;
	//HP
	uint32_t hp_ = 100;
	//発射タイマー
	int fireTimer = 0;
};

