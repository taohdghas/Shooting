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
    /// @brief プレイヤーと敵（および各弾）の衝突判定を行う。
    ///
    /// - プレイヤー弾と敵の衝突判定（ヒット時：enemy->TakeDamage(), bullet->OnCollision()）
    /// - 敵弾とプレイヤーの衝突判定（ヒット時：player->TakeDamage(), bullet->OnCollision()）
    /// - プレイヤー本体と敵本体の当たり判定（衝突時：両者のコールバック呼び出し）
    /// - player または enemy が nullptr の場合は何もしない
    ///
    /// @param player 判定対象の Player へのポインタ
    /// @param enemy  判定対象の Enemy へのポインタ
    void CheckPlayerEnemyCollisions(Player* player, Enemy* enemy);

private:
};
