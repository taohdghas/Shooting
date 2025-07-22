#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include <vector>

class TitleObject
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//プレイヤーオブジェクトの演出
	void PlayerObjDirection();
private:
	//タイトルのオブジェクト
	std::unique_ptr<Object3d>title;
	//pushspaceのオブジェクト
	std::unique_ptr<Object3d>pushspace;
	//プレイヤーオブジェクト
	std::unique_ptr<Object3d>playerobj;
	//プレイヤーオブジェクトTransform
	Transform playerobjTransform;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//playerObj回転速度
	const float RotateSpeed = 0.5f;
};

