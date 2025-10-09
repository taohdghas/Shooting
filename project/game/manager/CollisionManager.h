#pragma once
#include "Struct.h"
#include <list>


class Player;//プレイヤークラス前方宣言
class Enemy;//エネミークラス前方宣言
class playerBullet;//プレイヤー弾クラス前方宣言
class EnemyBullet;//敵弾クラス前方宣言
//衝突管理クラス
class CollisionManager
{
public:
	//playerとenemyの当たり判定
	void CheckPECollisions(Player*player,Enemy*enemy);
private:

};

