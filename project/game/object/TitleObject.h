#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include <vector>

// タイトルオブジェクト
class TitleObject {
public:
	/// <summary>
	/// タイトル画面に登場する全オブジェクトの初期化を行う
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレームの更新処理を行う
	/// </summary>
	void Update();

	/// <summary>
	/// タイトル、PUSH SPACE、プレイヤーモデルを描画する。
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーオブジェクトの演出処理を行う
	/// </summary>
	void PlayerObjDirection();

	/// <summary>
	/// デバッグ用UIを表示する。
	/// </summary>
	void Debug();

private:
	// タイトルのオブジェクト
	std::unique_ptr< MyEngine::Object3d> title_;
	// pushspaceのオブジェクト
	std::unique_ptr< MyEngine::Object3d> push_space_;
	// プレイヤーオブジェクト
	std::unique_ptr< MyEngine::Object3d> player_obj_;
	// プレイヤーオブジェクトTransform
	Transform player_obj_transform_;
	// Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	// playerObj回転速度
	const float kRotateSpeed = 0.5f;
	// ジャンプの速度
	float jump_velocity_ = 0.0f;
	// ジャンプの力
	float jump_power_ = 0.12f;
	// 最大ジャンプ数
	const int kMaxJumpCount = 2;
	// 重力
	float gravity_ = -0.01f;
	// 地面
	float ground_y_ = -0.5f;
	// ジャンプ間隔
	float jump_interval_ = 300.0f;
	// ジャンプタイマー
	float jump_timer_ = 0.0f;
	// 二段ジャンプ時回転速度
	float jump_rotate_speed_ = 180.0f;
	// ジャンプカウント
	int jump_count_ = 0;
	// ジャンプ中か
	bool is_jumping_ = false;
	// α値
	float alpha_ = 1.0f;
	// α値タイマー
	float alpha_timer_ = 0.0f;
};
