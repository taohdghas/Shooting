#pragma once
#include "BossState.h"

//ボスの強化状態を表すクラス
/// <summary>
/// ボスの強化状態における行動ロジックの実装
/// 状態遷移時の初期化処理、強化状態特有の攻撃・移動・弾発射などの毎フレーム更新処理（Update）の具体化
/// 扇状ショットや二段ショットなど強化状態専用の攻撃パターン・タイミング管理
/// Boss本体への操作の実行
/// 強化状態から他状態への遷移条件の判定
/// </summary>
class BossStateEnraged : public BossState {
public:
    /// <summary>
	/// 状態に入るときの処理
    /// </summary>
    void Enter(Boss1& boss) override;
    /// <summary>
	/// 状態の更新
    /// </summary>
    void Update(Boss1& boss) override;
private:
	// 発射タイマー
    int fire_timer_ = 0;
	// 発射間隔
    int fire_interval_current_ = 0;
	// 扇状ショット遅延タイマー
    int fan_shot_delay_timer_ = 0;
	// 二段ショット遅延タイマー
    int second_shot_delay_timer_ = 0;
	// 二段ショット発射保留フラグ
    bool is_second_shot_pending_ = false;
	// 扇状ショット発射保留フラグ
    bool isFanShotPending_ = false;
	// 扇状ショット遅延時間定数
    static const int kFanShotDelay = 30;
	// 二段ショット遅延時間定数
    static const int kSecondShotDelay = 15;
};