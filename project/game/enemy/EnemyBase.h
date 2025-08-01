#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "Struct.h"

class Player;
class EnemyBase
{
public:
	virtual ~EnemyBase() = default;
	//初期化
	virtual void Initialize(Object3dBase* object3dbase) = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Draw() = 0;
	//ダメージ
	virtual void TakeDamage(int damage) = 0;
	//デスフラグが立ったか
	virtual bool IsDead()const = 0;
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }
	//半径
	float GetRadius()const { return radius; }
	///Setter///
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	//Playerのポインタをセット
	void SetPlayer(Player* player) { player_ = player; }

protected:
	Object3dBase* object3dBase_;
	Player* player_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//色
	Vector4 color_;
	//体力
	int hp;
	//半径
	float radius;
	//デスフラグ
	bool isDead_ = false;
	//色変化時間
	const float damageColorDuration = 0.1f;
	//ダメージ色変化タイマー
	float damageColorTimer_ = 0.0f;
};

