#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"
#include "BossState.h"
#include <random>

//前方宣言
class Player;
/// <summary>
/// ボスキャラクターの状態管理
/// 状態パターンによる行動制御（移動・攻撃・待機等）
/// 弾の生成・管理・描画および攻撃パターンの実装
/// プレイヤーとの連携
/// ダメージ処理、死亡時の演出・パーティクル発生制御
/// 乱数生成による行動パターンや弾発射間隔の決定
/// デバッグUIの表示
/// 3Dオブジェクトやコリジョン（OBB）情報の管理
/// </summary>
class Boss1
{
  public:
	/// <summary>
	/// 初期化
	/// <param name="object3d_base">3Dオブジェクト共通設定へのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 更新処理
	/// <returns>なし</returns>
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();
	
	/// <summary>
	/// 移動
	/// <returns>なし</returns>
	/// </summary>
	void Move();

	/// <summary>
	/// 二段高さショットを発射
	/// <returns>なし</returns>
	/// </summary>
	void FireDoubleHeightShot();

	/// <summary>
	/// 次の移動目標を決定
	/// <returns>なし</returns>
	/// </summary>
	void DecideNextTarget();

	/// <summary>
    /// 扇状拡散ショットを発射
	/// <returns>なし</returns>
    /// </summary>
	void FireFanShot();

	/// <summary>
	/// 衝突時のコールバック
	/// <returns>なし</returns>
	/// </summary>
	void OnCollision();

	/// <summary>
	/// HP を減少
	/// <param name="damage">減少させるダメージ量</param>
	/// <returns>なし</returns>
	/// </summary>
	void TakeDamage(uint32_t damage);

	/// <summary>
	/// 状態を変更
	/// <param name="newState">新しい状態のユニークポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void ChangeState(std::unique_ptr<BossState> newState);

	/// <summary>
	/// デバッグ UIを表示
	/// <returns>なし</returns>
	/// </summary>
	void Debug();

	/// <summary>
	/// OBBを取得する
	/// <returns>OBB構造体</returns>
	/// </summary>
	OBB GetOBB() const; 

	/// <summary>
	/// 弾発射間隔を決定
	/// <returns>発射間隔（フレーム数）</returns>
	///	</summary>
	int DecideFireInterval();

	/// <summary>
	/// min から max の範囲で乱数の浮動小数点数を生成
	/// <param name="min">最小値</param>
	/// <param name="max">最大値</param>
	/// <returns>生成された乱数値</returns>
	/// </summary>
	float RandomFloat(float min, float max);

	/// <summary>
	/// タイマーの共通関数
	/// <param name="timer">タイマー変数（参照渡し）</param>
	/// <param name="threshold">閾値</param>
	/// <returns>タイマーが閾値を超えた場合true、それ以外はfalse</returns>
	/// </summary>
	bool UpdateDelayTimer(int& timer, int threshold);

	/// <summary>
	/// 死亡フラグが立っているか判定
	/// <returns>死亡していればtrue</returns>
	/// </summary>
	bool IsDead() const { return is_dead_; }

	/// <summary>
	/// 死亡時のパーティクル発生フラグが立っているか判定
	/// <returns>発生していればtrue</returns>
	/// </summary>
	bool IsDeathParticle() const { return is_death_particle_; }
public:
	/// <summary>
	/// 現在の位置（Transform.translate）を取得
	/// <returns>現在の座標（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetPosition() const { return transform_.translate; }

	/// <summary>
	/// 弾の攻撃力を取得
	/// <returns>攻撃力</returns>
	/// </summary>
	int GetAttack() const { return attack_; }

	/// <summary>
	/// 現在のHPを取得
	/// <returns>現在のHP</returns>
	/// </summary>
	int GetHP() const { return hp_; }

	/// <summary>
	/// ボス強化体力値を取得
	/// <returns>強化体力値</returns>
	/// </summary>
	int GetEnragedHP() const { return kEnragedHP; }

	/// <summary>
	/// コリジョンや描画に用いる半径を取得
	/// <returns>半径</returns>
	/// </summary>
	float GetRadius() const { return radius_; }

	/// <summary>
	/// 所有する弾リストを取得
	/// <returns>弾リスト（const参照）</returns>
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	/// <summary>
	/// スケールを設定
	/// <param name="scale">設定するスケール値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }

	/// <summary>
	/// 回転を設定
	/// <param name="rotate">設定する回転値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

	/// <summary>
	/// 座標を設定
	/// <param name="position">設定する座標値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }

	/// <summary>
	/// 対象プレイヤーのポインタを設定
	/// <param name="player">プレイヤーのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// デスパーティクル発生フラグを設定
	/// <param name="flag">設定するフラグ値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetIsDeathParticle(bool flag) { is_death_particle_ = flag; }

	/// <summary>
	/// 通常時のスケールを設定
	/// <param name="scale">設定するスケール値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetDefaultScale(const Vector3& scale) {default_scale_ = scale;}

	void SetMaxSpeed(float speed) { max_speed_ = speed; }
	void SetAcceleration(float accel) { acceleration_ = accel; }
private:
	// 現在の状態を表すステート
	std::unique_ptr<BossState> state_;
	// 3Dオブジェクト共通設定へのポインタ
	MyEngine::Object3dBase* object3d_base_ = nullptr;
	// 敵本体の3Dオブジェクト
	std::unique_ptr<MyEngine::Object3d> object_;
	// 敵が発射した弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 乱数生成器（Boss専用）
	std::mt19937 random_engine_;
	// 敵のワールド変換情報（位置・回転・スケール）
	Transform transform_ = {0.0f,0.0f,0.0f};
	// 攻撃対象となるプレイヤーのポインタ
	Player* player_ = nullptr;
	// 通常時のスケールを保存
	Vector3 default_scale_ = { 2.0f, 2.0f, 2.0f };
	//寸法
	Vector3 dimensions_ = { 4.0f,3.0f,2.0f };
	// 移動目標
	Vector3 target_position_ = {0.0f,0.0f,0.0f};
	// 速度
	Vector3 velocity_{ 0.0f, 0.0f, 0.0f };
	// 死亡フラグ
	bool is_dead_ = false;
	// 死亡時パーティクル発生フラグ
	bool is_death_particle_ = false;
	//発射間隔
	static const int kFireInterval = 200;
	// FireDoubleHeightShot用の高さオフセット
	static constexpr float kDoubleShotYOffset[3] = { -0.5f, 0.8f, 2.1f };
	// FireDoubleHeightShot用の横方向オフセット
	static constexpr float kDoubleShotXOffset[6] = { -3.0f, -1.8f, -0.6f, 0.6f, 1.8f, 3.0f };
	//現在のHP
	int hp_ = 190;
	//攻撃力
	int attack_ = 5;
	// 弾発射タイマーのカウント
	int fire_timer_count_ = 0;
	// コリジョンや描画に使う半径
	float radius_ = 1.0f;
	// 1フレームあたりの時間（秒）
	const float kDeltaTime = 1.0f / 60.0f;
	// ダメージ色の持続時間（秒）
	const float kDamageColorDuration = 0.1f;
	// ダメージスケール演出の時間
	const float kDamageScaleDuration = 0.08f;
	// 弾速
	const float kBulletSpeed = 0.7f;
	//ボス強化体力値
	const int kEnragedHP = 100;
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
};