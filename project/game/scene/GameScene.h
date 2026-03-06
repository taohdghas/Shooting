#pragma once
#include "Audio.h"
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
#include "Boss1.h"
#include "Skybox.h"
#include "Platform.h"
#include "Ui.h"

//ゲームシーン
/// <summary>
/// ゲーム進行全体の初期化・更新・描画・終了処理の管理
/// プレイヤー、敵、ボス、プラットフォーム、UI、カメラ、パーティクル、フェード等のリソース管理と連携
/// ゲームフェーズの制御と状態遷移
/// 衝突判定やレベルデータ、演出の制御
/// シーン遷移処理やゲーム進行の各種判定・演出の実装
/// 毎フレームの状態更新・描画・デバッグ表示の提供
/// </summary>
class GameScene : public  MyEngine::BaseScene
{
public:
	enum class GameStartState
	{
		None,
		SlideIn,
		Stop,
		SlideOut
	};
	//ゲームフェーズ
	enum class GamePhase {
		Stage,     // 通常進行
		BossBattle // ボス戦
	};

	/// <summary>
	/// 初期化
	/// <returns>なし</returns>
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// <returns>なし</returns>
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// <returns>なし</returns>
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示
	/// <returns>なし</returns>
	/// </summary>
	void Debug() override;

	/// <summary>
	/// 追従カメラ
	/// <returns>なし</returns>
	///	</summary>	
	void FollowCamera();

	/// <summary>
	/// スタート演出 
	/// <returns>なし</returns>
	/// </summary>
	void StartAnimation();

	/// <summary>
	/// ゲームクリアシーンへ遷移
	/// <returns>なし</returns>
	/// </summary>
	void ToGameClear();

	/// <summary>
	/// ゲームオーバーシーンへ遷移
	/// <returns>なし</returns>
	/// </summary>
	void ToGameOver();

	/// <summary>
	/// ボス出現条件を判定
	/// <returns>ボス出現条件を満たしていればtrue</returns>
	/// </summary>
	bool IsBossSpawnCondition();

private:
	GameStartState game_start_state_ = GameStartState::None;
	//ゲームフェーズ
	GamePhase game_phase_ = GamePhase::Stage;
	//プレイヤー
	std::unique_ptr<Player> player_;
	//敵
	std::vector<std::unique_ptr<Enemy>> enemies_;
	//ボス
	std::unique_ptr<Boss1>boss_;
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
	std::unique_ptr<LevelData> level_data_;
	//ゲームスタート文字スプライト
	std::unique_ptr<MyEngine::Sprite>game_start_sprite_;
	//ゲームスタート文字の位置
	Vector2 game_start_pos_;
	//ボス出現位置
	Vector3 boss_spawn_position_;
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
	//乱数分布
	std::uniform_real_distribution<float> random_dist_{ -1.0f, 1.0f };
	//Δtを定義（定数）
	const float kDeltaTime = 1.0f / 60.0f;
	//ボストリガーZ座標
	const float kBossTriggerZ = 150.0f;
	//ボス出現距離オフセット
	const float kBossSpawnDistance = 20.0f;
	//1回転にかかる時間 
	const float totalRotationTime = 5.0f;
	//角速度 
	const float rotationSpeed = 1.0f;
	//収束に書ける時間 
	const float oneRotation = 2.0f * 3.14159f;
	//スタート演出スライド速度
	const float kSlideSpeed = 20.0f;
	//スタート演出停止時間
	const float kStopTime = 1.5f;
	//スタート演出スライド時間
	const float kSlideDuration = 0.6f;
	//撃破演出経過時間タイマー
	float death_timer_ = 0.0f;
	//撃破演出回転速度
	float death_rotation_speed_ = 5.0f;
	//重力加速度
	float gravity_ = 0.015f;
	//Zオフセット
	float z_offset_ = 0.0f;
	//カメラ回転タイマー
	float camera_rotate_timer_ = 0.0f;
	// GAME START表示タイマー
	float game_start_timer_ = 0.0f;
	//画面中央のX座標
	float screen_center_x = 640.0f;
	//画面中央のY座標
	float screen_center_y = 360.0f;
	//ゲームスタートアニメーション時間
	float game_start_animation_time_ = 0.0f;
	//スタート演出スライド開始X座標
	float slide_start_x_ = 0.0f;
	//スタート演出スライド終了X座標
	float slide_end_x_ = 0.0f;
	//ゲームクリアシーン遷移フラグ
	bool is_to_game_clear_ = false;
	//ゲームオーバーシーン遷移フラグ
	bool is_to_game_over_ = false;
	//撃破演出が始まったか
	bool is_death_motion_started_ = false;
	//ボスが出現したか
	bool is_boss_spawned_ = false;
	//スタート演出フラグ
	bool is_start_animation_ = true;
	//リターン演出フラグ
	bool is_returning_ = false;
	//カメラ追従初期化フラグ
	bool is_following_initialized_ = false;
	//ゲーム一時停止フラグ
	bool is_game_pause_ = false;
	//ゲームプレイ中フラグ
	bool is_gameplay_active_ = false;
};