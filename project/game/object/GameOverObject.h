#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

//ゲームオーバーオブジェクト
class GameOverObject
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//デバック
	void Debug();
private:
	//ゲームオーバー字のオブジェクト
	std::unique_ptr<Object3d>gameover;
	//retryのオブジェクト
	std::unique_ptr<Object3d>retry;
	//ゲームオーバートランスフォーム
	Transform gameoverTransform;
	//retryトランスフォーム
	Transform retryTransform;
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;
	//α値
	float alpha_ = 1.0f;
	//α値タイマー
	float alphaTimer_ = 0.0f;
};

