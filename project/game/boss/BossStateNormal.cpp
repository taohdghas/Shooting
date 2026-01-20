#include "BossStateNormal.h"
#include "Boss1.h"
#include "BossStateEnraged.h"

// 状態に入るときの処理
void BossStateNormal::Enter(Boss1& boss) {
    fire_timer_ = 0;
    second_shot_delay_timer_ = 0;
    is_second_shot_pending_ = false;
	// 発射間隔決定
    fire_interval_current_ = boss.DecideFireInterval();
}
// 状態の更新
void BossStateNormal::Update(Boss1& boss) {
    //移動
    boss.Move();
	// 二段高さショット発射処理
    if (boss.UpdateDelayTimer(fire_timer_, fire_interval_current_)) {
        boss.FireDoubleHeightShot();
        is_second_shot_pending_ = true;
        fire_interval_current_ = boss.DecideFireInterval();
    }
	// 二段ショット遅延処理
    if (is_second_shot_pending_ &&
        boss.UpdateDelayTimer(second_shot_delay_timer_, kSecondShotDelay)) {
        boss.FireDoubleHeightShot();
        is_second_shot_pending_ = false;
    }

    // 強化遷移
    if (boss.GetHP() <= boss.GetEnragedHP()) {
        boss.ChangeState(std::make_unique<BossStateEnraged>());
    }
}