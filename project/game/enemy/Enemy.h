#pragma once
#include "EnemyBase.h"
#include "EnemyBullet.h"

// 雑魚敵クラス
class Enemy : public EnemyBase {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base) override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug(int id) override;

	/// <summary>
	/// 所有する弾リスト取得
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

	/// <summary>
	/// レール設定
	/// </summary>
	void SetRail(const std::vector<Vector3>& controlPoints, bool closed);

private:
	/// <summary>
	/// レーザー攻撃
	/// </summary>
	void Laser();

	/// <summary>
	/// レール上の位置評価
	/// </summary>
	Vector3 EvaluateRailPosition(float progress);

private:
	// ===== Enemy1 固有 =====

	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// レール移動
	std::vector<Vector3> rail_points_;
	bool rail_closed_ = false;

	Vector3 rail_base_position_;
	Vector3 rail_start_point_;
	Vector3 rail_lap_offset_;
	Vector3 rail_accumulated_;

	float rail_progress_ = 0.0f;
	float rail_speed_ = 7.0f;
	float rail_total_length_ = 0.0f;

	// 攻撃
	static const int kFireInterval = 80;
	const float kFireDistance = 25.0f;
	const float kAttackStopDistanceZ = 6.0f;

	int fire_timer_ = 0;
};
