#pragma once

#include "Object3d.h"
#include "Object3dBase.h"
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include "Struct.h"
#include "playerBullet.h"
#include "Camera.h"
#include <list>
#include <memory>
#include <vector>

//プラットフォーム
class Platform;
//プレイヤー
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(bool is_start_animation_, bool is_returning_);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// レティクル描画
	/// </summary>
	void ReticleDraw();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// 回避
	/// </summary>
	void Dodge();

	/// <summary>
	/// レティクル更新
	/// </summary>
	void ReticleUpdate();

	/// <summary>
	/// 衝突時処理
	/// </summary>
	void OnCollision();

	/// <summary>
	/// ダメージ処理
	/// </summary>
	void TakeDamage(int damage);

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// OBB取得
	/// </summary>
	OBB GetOBB() const;

	/// <summary>
	/// 死亡判定
	/// </summary>
	bool IsDead() const { return is_dead_; }

	/// <summary>
	/// スケール取得
	/// </summary>
	const Vector3& GetScale() const { return transform_.scale; }

	/// <summary>
	/// 回転取得
	/// </summary>
	const Vector3& GetRotate() const { return transform_.rotate; }

	/// <summary>
	/// 位置取得
	/// </summary>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// HP取得
	/// </summary>
	int GetHP() const { return hp_; }

	/// <summary>
	/// 半径取得
	/// </summary>
	float GetRadius() const { return radius_; }

	/// <summary>
	/// 弾リスト取得
	/// </summary>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	/// <summary>
	/// スケール設定
	/// </summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// 回転設定
	/// </summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	/// <summary>
	/// 座標設定
	/// </summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }

	/// <summary>
	/// プラットフォーム設定
	/// </summary>
	void SetPlatform(Platform* platform) { platform_ = platform; }

	/// <summary>
	/// プラットフォーム追従設定
	/// </summary>
	void SetFollowPlatform(bool flag) { is_follow_platform_ = flag; }

private:
	MyEngine::Object3dBase* object3d_base_;
	MyEngine::Camera* camera_;
	Transform transform_;
	//レティクル用
	Transform reticle_transform_;
	std::unique_ptr< MyEngine::Object3d> object_;
	//弾のリスト
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//レティクル
	std::unique_ptr< MyEngine::Sprite> reticle_;
	//プラットフォーム
	Platform* platform_ = nullptr;
	//画面上の位置
	Vector2 reticle_screen_pos_{ 640.0f, 360.0f };
	Vector2 reticle_pos_ = { 640.0f, 360.0f };
	//レティクルのオフセット
	Vector3 reticle_offset_{ 0.0f, 0.0f, 10.0f };
	//前フレームのプラットフォーム位置
	Vector3 prev_platform_pos_ = { 0, 0, 0 };
	//モデルの寸法
	Vector3 dimensions_ = { 2.0f,2.0f,2.0f };
	//色
	Vector4 color_;
	//回避方向
	Vector3 dodge_direction_{ 0.0f,0.0f,0.0f };
	//デスフラグ
	bool is_dead_ = false;
	//回避状態
	bool dodge_ = false;
	//ジャンプフラグ
	bool is_jumping_ = false;
	//無敵フラグ(デバック)
	bool is_invincible_ = false;
	//追従プラットフォーム初期化フラグ
	bool is_following_platform_initialized_ = false;
	//プラットフォーム追従フラグ
	bool is_follow_platform_ = true;
	//プレイヤーの移動速度
	float speed_ = 0.1f;
	//プレイヤーの半径
	float radius_ = 1.0f;
	//ジャンプ速度
	float jump_velocity_ = 0.0f;
	//HP
	int hp_ = 100;
	//ジャンプ回数
	int jump_count_ = 0;
	//最大ジャンプ可能回数
	const int kMaxJumpCount = 2;
	//弾の速度
	const float kBulletSpeed = 1.0f;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//回避速度
	const float dodge_speed_ = 0.1f;
	//回避時の回転
	const float rotate_angle_ = 360.0f;
	//地面の最小Y移動
	const float kGroundMinY = -3.0f;
	//地面の最小X移動
	const float kGroundMinX = -4.0f;
	//地面の最大X移動
	const float kGroundMaxX = 4.0f;
	//重力
	const float gravity_ = -0.01f;
	//ジャンプ力
	const float jump_power_ = 0.15f;
	//地面のY座標
	const float ground_y_ = -1.5f;
	//二段ジャンプ回転速度
	const float jump_rotate_speed_ = 720.0f;
	//色変化時間
	const float damage_color_duration_ = 0.1f;
	//無敵時間
	const float invincible_time_ = 0.1f;
	//攻撃のクールタイム
	float attack_cooldown_ = 0.0f;
	//再攻撃できるまでの間隔
	float attack_interval_ = 10.0f;
	//回避タイマー
	float dodge_timer_ = 0.0f;
	//回避適用時間
	float apply_dodge_ = 0.5f;
	//回避のクールタイム
	float dodge_cooldown_ = 0.0f;
	//回避コマンド再使用待機時間
	float dodge_interval_ = 5.0f;
	//ダメージ色変化タイマー
	float damage_color_timer_ = 0.0f;
	//無敵タイマー
	float invincible_timer_ = 0.0f;
};
