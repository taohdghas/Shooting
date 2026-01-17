#include "BossStateNormal.h"
#include "Boss1.h"
#include "BossStateEnraged.h"

void BossStateNormal::Enter(Boss1& boss) {
    fire_timer_ = 0;
    second_shot_delay_timer_ = 0;
    is_second_shot_pending_ = false;
    fire_interval_current_ = boss.DecideFireInterval();
}

void BossStateNormal::Update(Boss1& boss) {
    boss.Move();

    // 通常攻撃
    if (boss.UpdateDelayTimer(fire_timer_, fire_interval_current_)) {
        boss.FireDoubleHeightShot();
        is_second_shot_pending_ = true;
        fire_interval_current_ = boss.DecideFireInterval();
    }

    // 2段目
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