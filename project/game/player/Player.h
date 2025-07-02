#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Sprite.h"
#include "SpriteBase.h"
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
	//ジャンプ
	void Jump();
	//攻撃
	void Attack();
	//三方向攻撃
	void ThreeAttack();
	//回避
	void Dodge();
	//レティクル更新
	void ReticleUpdate();
	//衝突時コールバック
	void OnCollision();
	//HP減少関数
	void TakeDamage(int damage);
    //デバック表示
	void Debug();
	//デスフラグが立ったか
	bool IsDead()const { return isDead_; }
public:
	///Getter/// 

	//位置取得
	const Vector3& GetPosition()const { return transform_.translate; }
	//半径
	float GetRadius()const { return radius_; }

	//弾リスト取得
	const std::list<std::unique_ptr<playerBullet>>& GetBullets() const { return bullets_; }

	///Setter/// 
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	


private:
	Object3dBase* object3dBase_;
	Camera* camera_;
	Transform transform_;
	//レティクル用
	Transform reticleTransform_;
	std::unique_ptr<Object3d>object_;
	//弾のリスト
	std::list<std::unique_ptr<playerBullet>>bullets_;
	//レティクル
	std::unique_ptr<Sprite>reticle_;
	//回避の方向
	std::string dodgeDirection_;
	//デスフラグ
	bool isDead_ = false;
	//回避状態
	bool dodge_ = false;
	//ジャンプフラグ
	bool isJumping_ = false;
	//プレイヤーの移動速度
	float speed = 0.1f;
	//プレイヤーの半径
	float radius_ = 1.0f;
	//ジャンプ速度
	float jumpVelocity_ = 0.0f;
	//HP
	int hp_ = 100;
	//ジャンプ回数
	int jumpCount_ = 0;
	//最大ジャンプ可能回数
	const int maxJumpCount_ = 2;
	//弾の速度
	const float kBulletSpeed = 1.0f;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//回避速度
	const float dodgeSpeed_ = 0.2f;
	//回避時の回転
	const float rotateAngle_ = 1440.0f;
	//地面の最小Y移動
	const float groundminY = -3.0f;
	//地面の最小X移動
	const float groundminX = -4.0f;
	//地面の最大X移動
	const float groundmaxX = 4.0f;
	//重力
	const float gravity_ = -0.01f;
	//ジャンプ力
	const float jumpPower_ = 0.15f;
	//地面のY座標
	const float groundY_ = -1.5f;
	//二段ジャンプ回転速度
	const float jumpRotateSpeed_ = 720.0f;
	//攻撃のクールタイム
	float  attackCooldown_ = 0.0f;
	//再攻撃できるまでの間隔
	 float attackInterval_ = 10.0f;
	//回避タイマー
	float dodgeTimer_ = 0.0f;
	//回避適用時間
	float applyDodge_ = 1.0f;
	//回避のクールタイム
	float dodgeCooldown_ = 0.0f;
	//回避コマンド再使用待機時間
	float dodgeInterval_ = 5.0f;
};

