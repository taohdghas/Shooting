#pragma once
#include "BossState.h"
class BossStateEnraged : public BossState {
public:
    void Enter(Boss1& boss) override;
    void Update(Boss1& boss) override;
};