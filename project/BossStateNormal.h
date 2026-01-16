#pragma once
#include "BossState.h"

class BossStateNormal : public BossState
{
public:
    void Enter(Boss1& boss) override;
    void Update(Boss1& boss) override;
};