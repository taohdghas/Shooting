#pragma once
#include "DirectXBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "ModelManager.h"
#include "Input.h"
#include "Transform.h"

class Object3dBase;
class Player
{
public:
	//初期化
	void Initialize();
	//更新
	void Update(Input* input);
	//描画
	void Draw();
public:
	///Getter///
	const Vector3& GetPosition()const { return transform_.translate; }
	///Setter///

private:
	DirectXBase* directxBase_;
	Object3dBase* object3dBase_;
	//プレイヤーのオブジェクト3d
	std::unique_ptr<Object3d>object3d_;
	//トランスフォーム
	Transform transform_;
	//プレイヤーの移動速度
	float speed = 5.0f;
};