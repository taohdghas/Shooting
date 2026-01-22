#pragma once
#include "BossState.h"

//ボスの強化状態を表すクラス
// 通常状態と異なる移動・攻撃パターンを制御する
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