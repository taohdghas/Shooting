#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Input.h"
#include "Transform.h"
#include "playerBullet.h"
#include "Camera.h"
#include <list>
#include <memory>

class Player
{
public:
	//初期化
	void Initialize(Object3d*object);
	//更新
	void Update();
	//描画
	void Draw();
	//移動
	void Move();
	//攻撃
	void Attack();
private:
	Object3d* object_ = nullptr;
	Camera* camera_;
	Transform transform_;
	//弾のリスト
	std::list<std::unique_ptr<playerBullet>>bullets_;
	
	//移動速度
	float speed = 0.1f;
	//攻撃クールタイム
	int attackCooldown_ = 0;
	//攻撃間隔
	static const int attackInterval_ = 5;
};

