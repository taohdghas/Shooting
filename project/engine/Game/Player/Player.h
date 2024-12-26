#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Input.h"
#include "Transform.h"
#include "PlayerBullet.h"

class Object3dBase;
class Player
{
public:
	Player();

	~Player();

	//初期化
	void Initialize(Object3dBase*object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
	//攻撃
	void Attack();
	//衝突時コールバック関数
	void OnCollision();
public:
	//移動
	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }
	float GetRadius()const { return radius_; }
	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets()const { return bullets_; }
	///Setter///

private:
	Object3dBase* object3dBase_;
	Transform transform_;
	//Input* input_;
	//プレイヤーのオブジェクト3d
	std::unique_ptr<Object3d>object_;
	//弾のリスト
	std::list<PlayerBullet*>bullets_;
	//プレイヤーの移動速度
	float speed = 0.1f;
	//プレイヤーの半径
	float radius_ = 1.0f;
};