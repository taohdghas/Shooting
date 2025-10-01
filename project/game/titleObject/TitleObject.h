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
	//デバック
	void Debug();
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
	const float DeltaTime = 1.0f / 60.0f;
	//playerObj回転速度
	const float RotateSpeed = 0.5f;
    //ジャンプの速度
	float jumpVelocity_ = 0.0f;
	//ジャンプの力
	float jumpPower_ = 0.12f;
	//重力
	float gravity_ = -0.01f;
	//地面
	float groundY_ = -0.25f;  
	//ジャンプ間隔
	float jumpInterval_ = 300.0f; 
	//ジャンプタイマー
	float jumpTimer_ = 0.0f;
	//ジャンプ中か
	bool isJumping_ = false;

	float alpha_ = 1.0f;
	float alphaTimer_ = 0.0f;
};

