#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"
#include <random>

//前方宣言
class Player;
class Boss1
{
  public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// 二段高さショットを発射する
	/// </summary>
	void FireDoubleHeightShot();

	/// <summary>
	/// 次の移動目標を決定する
	/// </summary>
	void DecideNextTarget();

	/// <summary>
    /// 扇状拡散ショットを発射する
    /// </summary>
	void FireFanShot();

	/// <summary>
	/// 弾発射間隔を決定する
	///	</summary>
	int DecideFireInterval();

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	void OnCollision();
	/// <summary>
	/// HP を減少させる
	/// </summary>
	void TakeDamage(uint32_t damage);
	/// <summary>
	/// デバッグ UI を表示する（ImGui を使用）
	/// </summary>
	void Debug();
	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する
	/// </summary>
	OBB GetOBB() const;
	/// <summary>
	/// min から max の範囲で乱数の浮動小数点数を生成する
	/// </summary>
	float RandomFloat(float min, float max);

	/// <summary>
	/// 死亡フラグが立っているか判定する。
	/// </summary>
	bool IsDead() const { return is_dead_; }

	/// <summary>
	/// 死亡時のパーティクル発生フラグが立っているか判定する。
	/// </summary>
	bool IsDeathParticle() const { return is_death_particle_; }
public:
	/// <summary>
	/// 現在の位置（Transform.translate）を取得する。
	/// </summary>
	const Vector3& GetPosition() const { return transform_.translate; }

	/// <summary>
	/// 弾の攻撃力を取得する。
	/// </summary>
	int GetAttack() const { return attack_; }

	/// <summary>
	/// コリジョンや描画に用いる半径を取得する。
	/// </summary>
	float GetRadius() const { return radius_; }

	/// <summary>
	/// 所有する弾リストを取得する（読み取り専用）。
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	/// <summary>
	/// スケールを設定する。
	/// </summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// 回転を設定する。
	/// </summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }

	/// <summary>
	/// 対象プレイヤーのポインタを設定する。
	/// - 攻撃判定や追従に利用される。
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// デスパーティクル発生フラグを設定する。
	/// </summary>
	void SetIsDeathParticle(bool flag) { is_death_particle_ = flag; }

	/// <summary>
	/// 通常時のスケールを設定する。
	/// </summary>
	void SetDefaultScale(const Vector3& scale) {default_scale_ = scale;}
private:
	// 3Dオブジェクト共通設定へのポインタ
	MyEngine::Object3dBase* object3d_base_;
	// 敵本体の3Dオブジェクト
	std::unique_ptr<MyEngine::Object3d> object_;
	// 敵が発射した弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 乱数生成器（Boss専用）
	std::mt19937 random_engine_;
	// 敵のワールド変換情報（位置・回転・スケール）
	Transform transform_;
	// 攻撃対象となるプレイヤーのポインタ
	Player* player_;
	// 通常時のスケールを保存
	Vector3 default_scale_ = { 2.0f, 2.0f, 2.0f };
	//寸法
	Vector3 dimensions_ = { 4.0f,3.0f,2.0f };
	// 移動目標
	Vector3 target_position_;   
	// 速度
	Vector3 velocity_{ 0.0f, 0.0f, 0.0f };
	// 死亡フラグ
	bool is_dead_ = false;
	// 死亡時パーティクル発生フラグ
	bool is_death_particle_ = false;
	// 扇状拡散用ディレイ
	bool is_fan_shot_pending_ = false;
	//発射間隔
	static const int kFireInterval = 200;
	// ディレイ時間
	static const int kFanShotDelay = 30;
	//現在のHP
	int hp_ = 150;
	//攻撃力
	int attack_ = 5;
	// 弾発射タイマーのカウント
	int fire_timer_count_ = 0;
	// 弾発射タイマーの間隔管理
	int fire_timer_ = 0;
	// 扇状拡散ショット用ディレイタイマー
	int fan_shot_delay_timer_ = 0;
	// 次の弾発射までの間隔
	int fire_interval_current_ = kFireInterval;
	// コリジョンや描画に使う半径
	float radius_ = 1.0f;
	// 1フレームあたりの時間（秒）
	const float kDeltaTime = 1.0f / 60.0f;
	// ダメージ色の持続時間（秒）
	const float kDamageColorDuration = 0.1f;
	// ダメージスケール演出の時間
	const float kDamageScaleDuration = 0.08f;
	//ボス強化体力値
	const float kEnragedHP = 70.0f;
	// ダメージ色の残り時間
	float damage_color_timer_ = 0.0f;
	// ダメージスケール演出の時間
	float damage_scale_duration_ = 0.08f;
	// 現在のスケール演出の残り時間
	float damage_scale_timer_ = 0.0f;
	//移動速度
	float move_speed_ = 2.0f;
	// 到着判定の閾値
	float arrive_threshold_ = 0.2f;
	// 最大速度
	float max_speed_ = 4.0f;
	// 追従の滑らかさ
	float acceleration_ = 10.0f;
	// 浮遊用タイマー
	float float_time_ = 0.0f;
	// 速度減衰率（浮遊感用）
	float velocity_damping_ = 0.96f;
	// 次の目標までの最小距離
	float min_target_distance_ = 2.5f;
	// 2段目通常攻撃用
	bool is_second_shot_pending_ = false;
	int second_shot_delay_timer_ = 0;
	static const int kSecondShotDelay = 15; // 0.25秒くらい

};

