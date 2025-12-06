#include "CollisionManager.h"

#include "MyMath.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

// --- プレイヤー弾と敵の当たり判定処理を関数化 ---
void CollisionManager::CheckPlayerBulletsToEnemy(Player* player, Enemy* enemy) {
    const auto& playerBullets = player->GetBullets();
    if (!enemy->IsDead()) {
        for (const auto& bullet : playerBullets) {
            if (bullet->IsDead()) continue;
            OBB bulletOBB = bullet->GetOBB();
            OBB enemyOBB = enemy->GetOBB();
            if (Math::IsCollisionOBB(bulletOBB, enemyOBB)) {
                enemy->TakeDamage(bullet->GetAttack());
                bullet->OnCollision();
                if (enemy->IsDead()) {
                    enemy->SetIsDeathParticle(true);
                }
            }
        }
    }
}

// --- 敵弾とプレイヤーの当たり判定処理を関数化 ---
void CollisionManager::CheckEnemyBulletsToPlayer(Player* player, Enemy* enemy) {
    const auto& enemyBullets = enemy->GetBullets();
    OBB playerOBB = player->GetOBB();
    for (const auto& bullet : enemyBullets) {
        if (bullet->IsDead()) continue;
        OBB bulletOBB = bullet->GetOBB();
        if (Math::IsCollisionOBB(bulletOBB, playerOBB)) {
            player->TakeDamage(bullet->GetAttack());
            bullet->OnCollision();
        }
    }
}

// --- プレイヤー本体と敵本体の当たり判定処理を関数化 ---
void CollisionManager::CheckPlayerEnemyBody(Player* player, Enemy* enemy) {
    OBB playerOBB = player->GetOBB();
    OBB enemyOBB = enemy->GetOBB();
    if (Math::IsCollisionOBB(playerOBB, enemyOBB)) {
        player->OnCollision();
        enemy->OnCollision();
    }
}

// プレイヤーと敵の当たり判定処理
void CollisionManager::CheckPlayerEnemyCollisions(Player* player, Enemy* enemy) {
    if (!player || !enemy) {
        return;
    }
    CheckPlayerBulletsToEnemy(player, enemy);
    CheckEnemyBulletsToPlayer(player, enemy);
    CheckPlayerEnemyBody(player, enemy);
}
