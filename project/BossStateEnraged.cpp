#include "BossStateEnraged.h"
#include "Boss1.h"

void BossStateEnraged::Enter(Boss1& boss) {
    fire_timer_ = 0;
    second_shot_delay_timer_ = 0;
    is_second_shot_pending_ = false;
    fire_interval_current_ = boss.DecideFireInterval();

    isFanShotPending_ = false;
    fanShotDelayTimer_ = 0;
}

void BossStateEnraged::Update(Boss1& boss) {
    boss.Move();

    if (boss.UpdateDelayTimer(fire_timer_, fire_interval_current_)) {
        boss.FireDoubleHeightShot();
        is_second_shot_pending_ = true;
        fire_interval_current_ = boss.DecideFireInterval();
    }

    if (is_second_shot_pending_ &&
        boss.UpdateDelayTimer(second_shot_delay_timer_, kSecondShotDelay)) {
        boss.FireDoubleHeightShot();
        is_second_shot_pending_ = false;
    }

    if (!isFanShotPending_) {
        isFanShotPending_ = true;
    }

    if (isFanShotPending_ &&
        boss.UpdateDelayTimer(fanShotDelayTimer_, kFanShotDelay)) {
        boss.FireFanShot();
        isFanShotPending_ = false;
    }
}