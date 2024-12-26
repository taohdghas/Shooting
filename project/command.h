#pragma once
#include"player.h"

class Icommand
{
public:
	virtual void Exec(Player& player) = 0;
};

class MoveUpCommand :public Icommand {
public:
	void Exec(Player& player)override;
};

class MoveDownCommand :public Icommand {
public:
	void Exec(Player& player)override;
};

class MoveLeftCommand :public Icommand {
public:
	void Exec(Player& player)override;
};

class MoveRightCommand :public Icommand {
public:
	void Exec(Player& player)override;
};

class AttackCommand :public Icommand {
	void Exec(Player& player)override;
};
