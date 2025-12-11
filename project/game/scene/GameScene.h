#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "BaseScene.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "JsonManager.h"
#include "Fade.h"

#include <vector>
#include <random>

#include "Player.h"
#include "Enemy.h"
#include "Skybox.h"
#include "Platform.h"
#include "Ui.h"

//ゲームシーン
class GameScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// シーン初期化処理。
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// シーン終了処理。
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 毎フレーム更新処理。
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理。
	/// </summary>
	void Draw() override;
	/// <summary>
	/// デバッグ表示処理（ImGui）。
	/// </summary>
	void Debug() override;

	/// <summary>
	/// 追従カメラ
	///	</summary>	
	void FollowCamera();

	/// <summary>
	/// スタート演出 
	/// </summary>
	void StartAnimation();

	/// <summary>
	/// ゲームクリアシーンへ遷移する。
	/// </summary>
	void ToGameClear();

	/// <summary>
	/// ゲームオーバーシーンへ遷移する。
	/// </summary>
	void ToGameOver();

private:
	//プレイヤー
	std::unique_ptr<Player> player_;
	//敵
	std::vector<std::unique_ptr<Enemy>> enemies_;
	//Skybox
	std::unique_ptr<Skybox> skybox_;
	//プラットフォーム
	std::unique_ptr<Platform> platform_;
	//パーティクルエミッター
	std::vector<std::unique_ptr< MyEngine::ParticleEmitter>> particle_emitters_;
	//衝突マネージャー
	std::unique_ptr<CollisionManager> collision_manager_;
	//カメラ
	std::unique_ptr< MyEngine::Camera> camera_;
	//JsonManager
	std::unique_ptr< MyEngine::JsonManager> json_manager_;
	//フェード
	std::unique_ptr<Fade> fade_;
	//UI
	std::unique_ptr<Ui>ui_;
	//レベルデータ
	LevelData* level_data_ = nullptr;

	//撃破演出回転軸
	Vector3 death_rotation_axis_ = { 1.0f, 0.0f, 0.0f };
	//撃破演出速度
	Vector3 death_velocity_;
	//回転開始時のカメラ位置
	Vector3 camera_start_pos_;
	//回転開始時のカメラ回転 
	Vector3 camera_start_rot_;
	//乱数生成器
	std::mt19937 random_engine_{ std::random_device{}() };
	std::uniform_real_distribution<float> random_dist_{ -1.0f, 1.0f };

	//Δtを定義（定数）
	const float kDeltaTime = 1.0f / 60.0f;
	//ゲームオーバーシーン遷移フラグ
	bool is_to_game_over_ = false;
	//撃破演出が始まったか
	bool is_death_motion_started_ = false;
	//撃破演出経過時間タイマー
	float death_timer_ = 0.0f;
	//撃破演出回転速度
	float death_rotation_speed_ = 5.0f;
	//重力加速度
	float gravity_ = 0.015f;
	//1回転にかかる時間 
	const float totalRotationTime = 5.0f;
	//角速度 
	const float rotationSpeed = 1.0f;
	//収束に書ける時間 
	const float oneRotation = 2.0f * 3.14159f;
	//スタート演出フラグ
	bool is_start_animation_ = true;
	//リターン演出フラグ
	bool is_returning_ = false;
	//カメラ追従初期化フラグ
	bool is_following_initialized_ = false;
	//Zオフセット
	float z_offset_ = 0.0f;
	//カメラ回転タイマー
	float camera_rotate_timer_ = 0.0f;
};
