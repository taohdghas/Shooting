#include "CollisionManager.h"

#include "MyMath.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

// プレイヤーと敵の当たり判定処理
void CollisionManager::CheckPlayerEnemyCollisions(Player* player, Enemy* enemy) {
    if (!player || !enemy) {
        return;
    }

    // プレイヤーと敵の弾リスト取得
    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

    // ---------------------------------------------------------
    // プレイヤー弾 → 敵 の当たり判定
    // ---------------------------------------------------------
    if (!enemy->IsDead()) {
        for (const auto& bullet : playerBullets) {
            if (bullet->IsDead()) {
                continue;
            }

            OBB bulletOBB = bullet->GetOBB();
            OBB enemyOBB = enemy->GetOBB();

            if (Math::IsCollisionOBB(bulletOBB, enemyOBB)) {

                // 敵にダメージ付与
                enemy->TakeDamage(bullet->GetAttack());

                // 弾の衝突処理
                bullet->OnCollision();

                // 敵が死亡したらパーティクルフラグを立てる
                if (enemy->IsDead()) {
                    enemy->SetIsDeathParticle(true);
                }
            }
        }
    }

    // ---------------------------------------------------------
    // 敵弾 → プレイヤー の当たり判定
    // ---------------------------------------------------------
    OBB playerOBB = player->GetOBB();
    for (const auto& bullet : enemyBullets) {
        if (bullet->IsDead()) {
            continue;
        }

        OBB bulletOBB = bullet->GetOBB();

        if (Math::IsCollisionOBB(bulletOBB, playerOBB)) {

            // プレイヤーにダメージ付与
            player->TakeDamage(bullet->GetAttack());

            // 弾の衝突処理
            bullet->OnCollision();
        }
    }

    // ---------------------------------------------------------
    // プレイヤー本体 ↔ 敵本体 の当たり判定
    // ---------------------------------------------------------
    OBB enemyOBB = enemy->GetOBB();
    if (Math::IsCollisionOBB(playerOBB, enemyOBB)) {

        player->OnCollision();
        enemy->OnCollision();
    }
}
