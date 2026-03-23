#pragma once

#include "Object3d.h"
#include "Object3dBase.h"
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include "Struct.h"
#include "PlayerBullet.h"
#include "Camera.h"
#include <list>
#include <memory>
#include <vector>

class Enemy;
//プラットフォーム
class Platform;
//プレイヤー
/// <summary>
/// プレイヤーキャラクターの状態管理
/// 操作処理
/// 弾の生成・管理・描画
/// レティクルの表示・操作
/// プラットフォームとの連携
/// 衝突判定とダメージ処理
/// デバッグ情報の表示
/// カメラとの連携
/// </summary>
class Player
{
public:
	/// <summary>
	/// コンストラクタ
	/// <returns>なし</returns>
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// <returns>なし</returns>
	/// </summary>
	~Player();

	/// <summary>
	/// 初期化
	/// <param name="object3d_base">3Dオブジェクト共通設定へのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 更新
	/// <param name="is_start_animation_">開始アニメーション中か</param>
	/// <param name="is_returning_">リターン中か</param>
	/// <returns>なし</returns>
	/// </summary>
	void Update(bool is_control_enabled);

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// レティクル描画
	/// <returns>なし</returns>
	/// </summary>
	void ReticleDraw();

	/// <summary>
	/// 移動
	/// <returns>なし</returns>
	/// </summary>
	void Move();

	/// <summary>
	/// ジャンプ
	/// <returns>なし</returns>
	/// </summary>
	void Jump();

	/// <summary>
	/// 攻撃
	/// <returns>なし</returns>
	/// </summary>
	void Attack();

	/// <summary>
	/// 回避
	/// <returns>なし</returns>
	/// </summary>
	void Dodge();

	/// <summary>
	/// レティクル更新
	/// <returns>なし</returns>
	/// </summary>
	void ReticleUpdate();

	/// <summary>
	/// 衝突時処理
	/// <returns>なし</returns>
	/// </summary>
	void OnCollision();

	/// <summary>
	/// ダメージ処理
	/// <param name="damage">受けるダメージ量</param>
	/// <returns>なし</returns>
	/// </summary>
	void TakeDamage(int damage);

	/// <summary>
	/// デバッグ表示
	/// <returns>なし</returns>
	/// </summary>
	void Debug();

	/// <summary>
    /// 回避クールタイム進行率 
	/// <returns>回避クールタイム進行率（0.0～1.0）</returns>
    /// </summary>
	float GetDodgeCooldownRatio();

	/// <summary>
	/// OBB取得
	/// <returns>OBB構造体</returns>
	/// </summary>
	OBB GetOBB() const;

	/// <summary>
	/// 死亡判定
	/// <returns>死亡していればtrue</returns>
	/// </summary>
	bool IsDead() const { return is_dead_; }

	/// <summary>
	/// スケール取得
	/// <returns>スケール（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetScale() const { return transform_.scale; }

	/// <summary>
	/// 回転取得
	/// <returns>回転（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetRotate() const { return transform_.rotate; }

	/// <summary>
	/// 位置取得
	/// <returns>座標（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// 速度取得
	/// <returns>速度（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetVelocity() const { return velocity_; }

	/// <summary>
	/// HP取得
	/// <returns>現在のHP</returns>
	/// </summary>
	int GetHP() const { return hp_; }

	/// <summary>
	/// 半径取得
	/// <returns>半径</returns>
	/// </summary>
	float GetRadius() const { return radius_; }

	/// <summary>
	/// 弾リスト取得
	/// <returns>弾リスト（const参照）</returns>
	/// </summary>
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	/// <summary>
	/// スケール設定
	/// <param name="scale">設定するスケール値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// 回転設定
	/// <param name="rotate">設定する回転値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	/// <summary>
	/// 座標設定
	/// <param name="position">設定する座標値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }

	/// <summary>
	/// プラットフォーム設定
	/// <param name="platform">プラットフォームのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetPlatform(Platform* platform) { platform_ = platform; }

	/// <summary>
	/// プラットフォーム追従設定
	/// <param name="flag">追従フラグ</param>
	/// <returns>なし</returns>
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
	//敵のリスト
	std::vector<Enemy*> enemies_;
	//プラットフォーム
	Platform* platform_ = nullptr;
	//レティクル画面上の位置
	Vector2 reticle_screen_pos_{ 640.0f, 360.0f };
	//レティクルの位置
	Vector2 reticle_pos_ = { 640.0f, 360.0f };
	//プレイヤーの移動速度
	Vector3 velocity_{ 0.0f, 0.0f, 0.0f };
	//レティクルのオフセット
	Vector3 reticle_offset_{ 0.0f, 0.0f, 10.0f };
	//前フレームのプラットフォーム位置
	Vector3 prev_platform_pos_ = { 0, 0, 0 };
	//モデルの寸法
	Vector3 dimensions_ = { 2.0f,2.0f,2.0f };
	//色
	Vector4 color_;
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
	//プレイヤーの半径
	float radius_ = 1.0f;
	//ジャンプ速度
	float jump_velocity_ = 0.0f;
	//キー入力による移動速度
	float move_speed_x_ = 0.1f;
	//HP
	int hp_ = 100;
	//ジャンプ回数
	int jump_count_ = 0;
	//最大ジャンプ可能回数
	const int kMaxJumpCount = 2;
	// 弾速度
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
	const float kGroundMinX = -3.5f;
	//地面の最大X移動
	const float kGroundMaxX = 3.5f;
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
	const float invincible_time_ = 0.2f;
	//回避最大回転量
	const float dodge_max_rotate_y_ = 25.0f;
	//攻撃のクールタイム
	float attack_cooldown_ = 0.0f;
	//再攻撃できるまでの間隔
	float attack_interval_ = 20.0f;
	//回避タイマー
	float dodge_timer_ = 0.0f;
	//回避適用時間
	float apply_dodge_ = 0.5f;
	//回避のクールタイム
	float dodge_cooldown_ = 0.0f;
	//回避コマンド再使用待機時間
	float dodge_interval_ = 5.0f;
	//回避回転量
	float dodge_rotate_y_ = 0.0f;
	//回避ジャンプの高さ
	float dodge_jump_height_ = 0.5f;
	//回避開始時のY座標
	float dodge_start_y_ = 0.0f;
	//ダメージ色変化タイマー
	float damage_color_timer_ = 0.0f;
	//無敵タイマー
	float invincible_timer_ = 0.0f;
};