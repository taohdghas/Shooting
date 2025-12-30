#pragma once

#include "Struct.h"
#include <list>

// 前方宣言
class Player;
class Enemy;
class Boss1;
class PlayerBullet;
class EnemyBullet;

// 衝突管理クラス
class CollisionManager {
public:
	/// <summary>
	/// プレイヤーと敵（および各弾）の衝突判定を行う。
	/// </summary>
	void CheckPlayerEnemyCollisions(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー弾と敵の当たり判定を行う。
	/// </summary>
	void CheckPlayerBulletsToEnemy(Player* player, Enemy* enemy);

	/// <summary>
	/// 敵弾とプレイヤーの当たり判定を行う。
	/// </summary>
	void CheckEnemyBulletsToPlayer(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー本体と敵本体の当たり判定を行う。
	/// </summary>
	void CheckPlayerEnemyBody(Player* player, Enemy* enemy);

	/// <summary>
    /// プレイヤーとボスと各弾の衝突判定を行う。
    /// </summary>
	void CheckPlayerBossCollisions(Player* player, Boss1*boss);

	/// <summary>
	/// プレイヤー弾とボスの当たり判定を行う。
	/// </summary>
	void CheckPlayerBulletsToBoss(Player* player,Boss1*boss);

	/// <summary>
	/// ボス弾とプレイヤーの当たり判定を行う。
	/// </summary>
	void CheckBossBulletsToPlayer(Player* player, Boss1*boss);

	/// <summary>
	/// プレイヤー本体とボス本体の当たり判定を行う。
	/// </summary>
	void CheckPlayerBossBody(Player* player, Boss1*boss);

private:
};
