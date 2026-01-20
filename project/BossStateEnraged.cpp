#include "BossStateEnraged.h"
#include "Boss1.h"

// 状態に入るときの処理
void BossStateEnraged::Enter(Boss1& boss) {
    fire_timer_ = 0;
    second_shot_delay_timer_ = 0;
    is_second_shot_pending_ = false;
	// 発射間隔決定
    fire_interval_current_ = boss.DecideFireInterval();

    isFanShotPending_ = false;
    fanShotDelayTimer_ = 0;
}
// 状態の更新
void BossStateEnraged::Update(Boss1& boss) {
	//移動
    boss.Move();

	// 二段高さショット発射処理
    if (!isFanShotPending_) {
        isFanShotPending_ = true;
    }
	// 二段ショット遅延処理
    if (isFanShotPending_ &&
        boss.UpdateDelayTimer(fanShotDelayTimer_, kFanShotDelay)) {
        boss.FireFanShot();
        isFanShotPending_ = false;
    }
}