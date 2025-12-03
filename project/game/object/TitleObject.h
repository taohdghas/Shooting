#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include <vector>

//タイトルオブジェクト
class TitleObject
{
public:
	/// <summary>
	/// タイトル画面に登場する全オブジェクトの初期化を行う。
	/// モデルの読み込み、スケール・位置の設定などを行う。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// タイトルや「PUSH SPACE」の点滅、プレイヤー演出を更新する。
	/// </summary>
	void Update();

	/// <summary>
	/// タイトル、PUSH SPACE、プレイヤーモデルを描画する。
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーオブジェクトの演出処理を行う。
	/// 回転やジャンプ、二段ジャンプなどの動きを制御する。
	/// </summary>
	void PlayerObjDirection();

	/// <summary>
	/// デバッグ用UIを表示する。
	/// ImGuiでスケール・回転・位置をリアルタイム調整可能にする。
	/// </summary>
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
	//最大ジャンプ数
	const int maxJumpCount_ = 2;
	//重力
	float gravity_ = -0.01f;
	//地面
	float groundY_ = -0.5f;  
	//ジャンプ間隔
	float jumpInterval_ = 300.0f; 
	//ジャンプタイマー
	float jumpTimer_ = 0.0f;
	//二段ジャンプ時回転速度
	float jumpRotateSpeed_ = 180.0f;
	//ジャンプカウント
	int jumpCount_ = 0;

	//ジャンプ中か
	bool isJumping_ = false;
	//α値
	float alpha_ = 1.0f;
	//α値タイマー
	float alphaTimer_ = 0.0f;

};

