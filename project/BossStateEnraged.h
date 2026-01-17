#pragma once
#include "BossState.h"
class BossStateEnraged : public BossState {
public:
    void Enter(Boss1& boss) override;
    void Update(Boss1& boss) override;
private:
    int fire_timer_ = 0;
    int fire_interval_current_ = 0;

    bool is_second_shot_pending_ = false;
    int second_shot_delay_timer_ = 0;

    static const int kSecondShotDelay = 15;

    bool isFanShotPending_ = false;
    int fanShotDelayTimer_ = 0;
    static const int kFanShotDelay = 30;
};