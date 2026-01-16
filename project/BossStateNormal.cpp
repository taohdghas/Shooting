#include "BossStateNormal.h"
#include "Boss1.h"
#include "BossStateEnraged.h"

void BossStateNormal::Enter(Boss1& boss) {
    // 必要なら初期化
}

void BossStateNormal::Update(Boss1& boss) {
    boss.Move();
    boss.Attack();

    // 強化状態へ遷移
    if (boss.GetHP() <= boss.GetEnragedHP()) {
        boss.ChangeState(std::make_unique<BossStateEnraged>());
    }
}