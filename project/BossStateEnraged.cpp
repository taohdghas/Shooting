#include "BossStateEnraged.h"
#include "Boss1.h"

void BossStateEnraged::Enter(Boss1& boss) {
  
}

void BossStateEnraged::Update(Boss1& boss) {
    boss.Move();

    boss.SetFanShotPending(true);

    boss.Attack(); 
}