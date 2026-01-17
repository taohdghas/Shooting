#pragma once
class Boss1;

class BossState {
public:
    virtual ~BossState() = default;
    virtual void Enter(Boss1& boss) {}
    virtual void Update(Boss1& boss) = 0;
    virtual void Exit(Boss1& boss) {}
};