#pragma once
#include "Struct.h"
#include <list>

class Player;
class Enemy;
class playerBullet;
class EnemyBullet;
class CollisionManager
{
public:
	//playerとenemyの当たり判定
	void CheckPECollisions(Player*player,Enemy*enemy);
private:

};

