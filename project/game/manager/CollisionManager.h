#pragma once

#include "Struct.h"
#include <list>

// 前方宣言
class Player;
class Enemy;
class PlayerBullet;
class EnemyBullet;

// 衝突管理クラス
class CollisionManager {
public:
    /// <summary>
    /// プレイヤーと敵（および各弾）の衝突判定を行う。
    /// - プレイヤー弾と敵の衝突判定（ヒット時：enemy->TakeDamage(), bullet->OnCollision()）
    /// - 敵弾とプレイヤーの衝突判定（ヒット時：player->TakeDamage(), bullet->OnCollision()）
    /// - プレイヤー本体と敵本体の当たり判定（衝突時：両者のコールバック呼び出し）
    /// - player または enemy が nullptr の場合は何もしない
    /// </summary>
    /// <param name="player">判定対象の Player へのポインタ</param>
    /// <param name="enemy">判定対象の Enemy へのポインタ</param>
    void CheckPlayerEnemyCollisions(Player* player, Enemy* enemy);

    /// <summary>
    /// プレイヤー弾と敵の当たり判定を行う。
    /// - 死亡していない敵に対して、プレイヤー弾がヒットした場合ダメージ処理・弾の衝突処理を行う。
    /// - 敵が死亡した場合はデスパーティクルフラグをセットする。
    /// </summary>
    /// <param name="player">判定対象の Player へのポインタ</param>
    /// <param name="enemy">判定対象の Enemy へのポインタ</param>
    void CheckPlayerBulletsToEnemy(Player* player, Enemy* enemy);

    /// <summary>
    /// 敵弾とプレイヤーの当たり判定を行う。
    /// - 敵弾がプレイヤーにヒットした場合、ダメージ処理・弾の衝突処理を行う。
    /// </summary>
    /// <param name="player">判定対象の Player へのポインタ</param>
    /// <param name="enemy">判定対象の Enemy へのポインタ</param>
    void CheckEnemyBulletsToPlayer(Player* player, Enemy* enemy);

    /// <summary>
    /// プレイヤー本体と敵本体の当たり判定を行う。
    /// - OBB同士の衝突判定が成立した場合、両者の OnCollision() を呼び出す。
    /// </summary>
    /// <param name="player">判定対象の Player へのポインタ</param>
    /// <param name="enemy">判定対象の Enemy へのポインタ</param>
    void CheckPlayerEnemyBody(Player* player, Enemy* enemy);

private:
};
