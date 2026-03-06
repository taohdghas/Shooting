#include "BossStateEnraged.h"
#include "Boss1.h"

// 状態に入るときの処理
void BossStateEnraged::Enter(Boss1& boss) {
    //各変数初期化
    fire_timer_ = 0;
    second_shot_delay_timer_ = 0;
    fan_shot_delay_timer_ = 0;

    is_second_shot_pending_ = false;
    isFanShotPending_ = false;
    
    fire_interval_current_ = boss.DecideFireInterval();

    //強化状態：移動速度アップ
    boss.SetMaxSpeed(8.0f);  
}
// 状態の更新
void BossStateEnraged::Update(Boss1& boss) {
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
	// 扇状ショット発射処理
    if (!isFanShotPending_) {
        isFanShotPending_ = true;
    }
	// 扇状ショット遅延処理
    if (isFanShotPending_ &&
        boss.UpdateDelayTimer(fan_shot_delay_timer_, kFanShotDelay)) {
        boss.FireFanShot();
        isFanShotPending_ = false;
    }
}