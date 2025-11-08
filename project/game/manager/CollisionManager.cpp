#include "CollisionManager.h"
#include "MyMath.h"

#include "Player.h"
#include "Enemy.h"
#include "playerBullet.h"
#include "EnemyBullet.h"

// プレイヤーと敵の当たり判定処理
void CollisionManager::CheckPECollisions(Player* player, Enemy* enemy) {
    if (!player || !enemy) return;

    // プレイヤー・敵の弾リスト取得
    const std::list<std::unique_ptr<playerBullet>>& playerBullets = player->GetBullets();
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

    // プレイヤー弾と敵の当たり判定
    if (!enemy->IsDead()) {
        for (const auto& bullet : playerBullets) {
            if (bullet->IsDead()) continue;

            // OBB（回転付きボックス）同士の衝突判定
            OBB bulletOBB = bullet->GetOBB();
            OBB enemyOBB = enemy->GetOBB();

            if (Math::IsCollisionOBB(bulletOBB, enemyOBB)) {
                // 敵にダメージを与え、弾を消す
                enemy->TakeDamage(bullet->GetAttack());
                bullet->OnCollision();

                // 敵が死亡した場合、デスパーティクルフラグを立てる
                if (enemy->IsDead()) {
                    enemy->SetIsDeathParticle(true);
                }
            }
        }
    }

    // 敵弾とプレイヤーの当たり判定
    OBB playerOBB = player->GetOBB();
    for (const auto& bullet : enemyBullets) {
        if (bullet->IsDead()) continue;

        OBB bulletOBB = bullet->GetOBB();
        if (Math::IsCollisionOBB(bulletOBB, playerOBB)) {
            // プレイヤーにダメージを与え、弾を消す
            player->TakeDamage(bullet->GetAttack());
            bullet->OnCollision();
        }
    }

    // プレイヤー本体と敵本体の当たり判定
    OBB enemyOBB = enemy->GetOBB(); // 再利用
    if (Math::IsCollisionOBB(playerOBB, enemyOBB)) {
        // 両者の衝突コールバックを呼ぶ
        player->OnCollision();
        enemy->OnCollision();
    }
}