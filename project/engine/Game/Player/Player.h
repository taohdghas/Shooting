#pragma once
#include "DirectXBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Input.h"
#include "Transform.h"
#include "PlayerBullet.h"

class Object3dBase;
class Player
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//攻撃
	void Attack();
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }
	///Setter///

private:
	DirectXBase* directxBase_;
	Object3dBase* object3dBase_;
	Model* model_;
	Transform transform_;
	Input* input_;
	//プレイヤーのオブジェクト3d
	Object3d* object3d_;
	//std::unique_ptr<Object3d>object3d_;
	//弾のリスト
	std::list<PlayerBullet*>bullets_;
	//プレイヤーの移動速度
	float speed = 5.0f;
};