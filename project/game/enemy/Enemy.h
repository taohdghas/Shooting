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
	// 所有する弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	// レール移動
	std::vector<Vector3> rail_points_;
	// レール基準位置
	Vector3 rail_base_position_;
	// レール開始点
	Vector3 rail_start_point_;
	// レール一周分のオフセット
	Vector3 rail_lap_offset_;
	// レール移動の累積オフセット
	Vector3 rail_accumulated_;
	// レールが閉じているか
	bool rail_closed_ = false;
	// レール進行度合い
	float rail_progress_ = 0.0f;
	// レール速度
	float rail_speed_ = 7.0f;
	// レール全長
	float rail_total_length_ = 0.0f;
	// 攻撃
	static const int kFireInterval = 80;
	const float kFireDistance = 25.0f;
	const float kAttackStopDistanceZ = 6.0f;
	// 攻撃タイマー
	int fire_timer_ = 0;
};
