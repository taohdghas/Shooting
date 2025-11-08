#pragma once
#include "EnemyBase.h"
#include "EnemyBullet.h"

// プレイヤークラス前方宣言
class Player;

/// <summary>
/// 雑魚敵（通常敵）クラス。
/// EnemyBase から継承し、個別の移動・攻撃（レーザー発射）ロジックを実装する。
/// </summary>
class Enemy : public EnemyBase
{
public:
	Enemy() = default;
	~Enemy() override = default;

	/// <summary>
	/// 初期化処理。
	/// - モデルを "enemy/enemy.obj" に設定。
	/// - スケール、位置、ライト無効などを設定。
	/// </summary>
	void Initialize(Object3dBase* object3dBase) override;

	/// <summary>
	/// 更新処理。
	/// - 弾の更新・削除。
	/// - 移動処理。
	/// - 攻撃（Laser）処理。
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグUIの描画。
	/// </summary>
	void Debug(int id) override;

	/// <summary>
	/// 衝突時処理。
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ダメージを受けたときの処理。
	/// </summary>
	void TakeDamage(int damage) override;

	/// <summary>
    /// 当たり判定用OBBを取得。
    /// </summary>
	OBB GetOBB() const override;

	/// <summary>
	/// レーザー攻撃（弾発射）。
	/// プレイヤーとの距離やタイマーをチェックして弾を生成。
	/// </summary>
	void Laser();

	/// <summary>
	/// 弾リストの取得。
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

private:
	// 弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 移動速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 発射関連
	int fireTimerCount_ = 0;
	int fireTimer_ = 0;
	static const int kFireInterval = 80;

	// 発射距離
	const float fireDistance_ = 25.0f;
	// 攻撃停止距離Z
	const float attackStopDistanceZ_ = 6.0f;

	// モデル寸法
	float dimensions_ = 2.0f;
};
