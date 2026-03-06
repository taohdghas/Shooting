#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"

// プレイヤークラス前方宣言
class Player;

// 敵キャラクタークラス
/// <summary>
/// 敵キャラクターの状態管理
/// 移動、攻撃、弾発射など敵固有の行動ロジックの実装
/// プレイヤーや他オブジェクトとの当たり判定・ダメージ処理
/// 弾の生成・管理・描画
/// 死亡時の演出やリスポーン等の制御
/// 毎フレームの更新・描画処理の提供
/// デバッグ用情報の表示
/// </summary>
class Enemy {
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
	void Update(bool is_control_enabled_);

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// レーザー攻撃
	/// <returns>なし</returns>
	/// </summary>
	void Laser();

	/// <summary>
	/// 衝突時のコールバック
	/// <returns>なし</returns>
	/// </summary>
	void OnCollision();

	/// <summary>
	/// HPを減少
	/// <param name="damage">減少させるダメージ量</param>
	/// <returns>なし</returns>
	/// </summary>
	void TakeDamage(int damage);

	/// <summary>
	/// デバッグ UIを表示
	/// <param name="id">デバッグUI識別用ID</param>
	/// <returns>なし</returns>
	/// </summary>
	void Debug(int index);

	/// <summary>
	/// レール上の位置を評価
	/// <param name="progress">レール上の進行度（0.0～1.0）</param>
	/// <returns>評価された座標（Vector3）</returns>
	/// </summary>
	Vector3 EvaluateRailPosition(float progress);

	/// <summary>
	/// OBBを取得
	/// <returns>OBB構造体</returns>
	/// </summary>
	OBB GetOBB() const;

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
	/// 現在の位置を取得
	/// <returns>現在の座標（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
    /// 弾の攻撃力を取得
	/// <returns>攻撃力</returns>
    /// </summary>
	int GetAttack() const { return attack_; }

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
    /// 初期HPを設定
	/// <param name="hp">設定するHP値</param>
	/// <returns>なし</returns>
    /// </summary>
	void SetHP(int hp) { hp_ = hp; }

	/// <summary>
	/// 対象プレイヤーのポインタを設定
	/// - 攻撃判定や追従に利用される。
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
	/// 敵の移動用レール（スプライン）の制御点と閉じているかどうかを設定
	/// <param name="controlPoints">制御点リスト</param>
	/// <param name="closed">レールが閉じているかどうか</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetRail(const std::vector<Vector3>& controlPoints, bool closed);
private:
	// 3Dオブジェクト共通設定へのポインタ
	MyEngine::Object3dBase* object3d_base_;
	// 敵本体の3Dオブジェクト
	std::unique_ptr<MyEngine::Object3d> object_;
	// 敵のワールド変換情報
	Transform transform_;
	// 攻撃対象となるプレイヤーのポインタ
	Player* player_;
	// 敵が発射した弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// 移動用レールの制御点リスト
	std::vector<Vector3> rail_points_;
	// レール移動時の相対ベクトル
	std::vector<Vector3> relative_vectors_;
	// 移動速度ベクトル
	Vector3 velocity_ = { 0.0f, 0.0f };
	// 離脱速度
	Vector3 exit_velocity_ = { 0.0f,6.0f,0.0f };
	// 通常時のスケールを保存
	Vector3 default_scale_ = { 0.8f, 0.8f, 0.8f };
	// レール移動用基準位置
	Vector3 rail_base_position_;  
	// レール移動用開始点
	Vector3 rail_start_point_;    
	// レール移動用1周分のオフセット
	Vector3 rail_lap_offset_;   
	// レール移動時の累積オフセット
	Vector3 rail_accumulated_; 
	//寸法
	Vector3 dimensions_ = { 2.0f,2.0f,2.0f };
	// 現在の色
	Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};
	// 死亡フラグ
	bool is_dead_ = false;
	// 離脱中フラグ
	bool is_exiting_ = false;
	// 死亡時パーティクル発生フラグ
	bool is_death_particle_ = false;
	// レールが閉じているかどうか
	bool rail_closed_ = false;
	// 現在のHP
	int hp_ = 5;
	//攻撃力
	uint32_t attack_ = 5;
	// 弾発射タイマーのカウント
	int fire_timer_count_ = 0;
	// 弾発射タイマーの間隔管理
	int fire_timer_ = 0;
	// コリジョンや描画に使う半径
	float radius_ = 1.0f;
	// 弾発射のインターバル
	static const int kFireInterval = 40;
	// プレイヤーへの発射有効距離
	const float kFireDistance = 45.0f;
	// Z方向の攻撃停止距離
	const float kAttackStopDistanceZ = 6.0f;
	// 1フレームあたりの時間（秒）
	const float kDeltaTime = 1.0f / 60.0f;
	// ダメージ色の持続時間（秒）
	const float kDamageColorDuration = 0.1f;
	// ダメージスケール演出の時間
	const float kDamageScaleDuration = 0.08f;
	// プレイヤー後方に到達したと見なすZオフセット
	const float kVanishOffsetZ = 5.0f;
	// 画面外判定用Y
	const float kExitLimitY = 20.0f;
	// ダメージ色の残り時間
	float damage_color_timer_ = 0.0f;
	// レール上の進行度（0.0～1.0）
	float rail_progress_ = 0.0f;
	// レール移動速度
	float rail_speed_ = 5.0f;
	// ダメージスケール演出の時間
	float damage_scale_duration_ = 0.08f;
	// 現在のスケール演出の残り時間
	float damage_scale_timer_ = 0.0f;
	// レール全体の長さ
	float rail_total_length_ = 0.0f;
};