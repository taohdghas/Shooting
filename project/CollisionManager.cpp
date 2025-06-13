#include "CollisionManager.h"
#include "Math.h"

#include "Player.h"
#include "Enemy.h"
#include "playerBullet.h"
#include "EnemyBullet.h"


//playerとenemyの当たり判定
void CollisionManager::CheckPECollisions(Player* player, Enemy* enemy) {
    if (!player || !enemy) return;

    // プレイヤーと敵の弾の取得
    const std::list<std::unique_ptr<playerBullet>>& playerBullets = player->GetBullets();
    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

    // --- プレイヤー弾 vs 敵 ---
    if (!enemy->IsDead()) {
        for (const auto& bullet : playerBullets) {
            if (bullet->IsDead()) continue;

            float length = Math::Length(enemy->GetPosition() - bullet->GetPosition());
            float combinedRadius = enemy->GetRadius() + bullet->GetRadius();
            if (length <= combinedRadius) {
                enemy->TakeDamage(bullet->GetAttack());
                bullet->OnCollision();
            }
        }
    }

    // --- 敵弾 vs プレイヤー ---
    Vector3 posPlayer = player->GetPosition();
    for (const auto& bullet : enemyBullets) {
        if (bullet->IsDead()) continue;

        float length = Math::Length(bullet->GetPosition() - posPlayer);
        float combinedRadius = player->GetRadius() + bullet->GetRadius();
        if (length <= combinedRadius) {
            player->TakeDamage(bullet->GetAttack());
            bullet->OnCollision();
        }
    }

    // --- プレイヤー vs 敵 ---
    float length = Math::Length(enemy->GetPosition() - posPlayer);
    float combinedRadius = player->GetRadius() + enemy->GetRadius();
    if (length <= combinedRadius) {
        player->OnCollision();
        enemy->onCollision();
    }
}