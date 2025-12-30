#include "CollisionManager.h"

#include "MyMath.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss1.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"

// プレイヤーと敵の当たり判定処理
void CollisionManager::CheckPlayerEnemyCollisions(Player* player, Enemy* enemy) {
    if (!player || !enemy) {
        return;
    }
    CheckPlayerBulletsToEnemy(player, enemy);
    CheckEnemyBulletsToPlayer(player, enemy);
    CheckPlayerEnemyBody(player, enemy);
}

//プレイヤー弾と敵の当たり判定処理を関数化
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

//敵弾とプレイヤーの当たり判定処理を関数化
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

//プレイヤー本体と敵本体の当たり判定処理を関数化
void CollisionManager::CheckPlayerEnemyBody(Player* player, Enemy* enemy) {
    OBB playerOBB = player->GetOBB();
    OBB enemyOBB = enemy->GetOBB();
    if (Math::IsCollisionOBB(playerOBB, enemyOBB)) {
        player->TakeDamage(enemy->GetAttack());
    }
}

/// プレイヤーとボスと各弾の衝突判定を行う。
void CollisionManager::CheckPlayerBossCollisions(Player* player, Boss1* boss) {
    if (!player || !boss) {
        return;
    }
	CheckPlayerBulletsToBoss(player, boss);
	CheckBossBulletsToPlayer(player, boss);
	CheckPlayerBossBody(player, boss);
}

/// プレイヤー弾とボスの当たり判定を行う。
void CollisionManager::CheckPlayerBulletsToBoss(Player* player, Boss1* boss) {
    const auto& playerBullets = player->GetBullets();
    if (!boss->IsDead()) {
        for (const auto& bullet : playerBullets) {
            if (bullet->IsDead()) continue;
            OBB bulletOBB = bullet->GetOBB();
            OBB bossOBB = boss->GetOBB();
            if (Math::IsCollisionOBB(bulletOBB, bossOBB)) {
                boss->TakeDamage(bullet->GetAttack());
                bullet->OnCollision();
                if (boss->IsDead()) {
                    boss->SetIsDeathParticle(true);
                }
            }
        }
    }
}

/// ボス弾とプレイヤーの当たり判定を行う。
void CollisionManager::CheckBossBulletsToPlayer(Player* player, Boss1* boss) {
    const auto& bossBullets = boss->GetBullets();
    OBB playerOBB = player->GetOBB();
    for (const auto& bullet : bossBullets) {
        if (bullet->IsDead()) continue;
        OBB bossbulletOBB = bullet->GetOBB();
        if (Math::IsCollisionOBB(bossbulletOBB, playerOBB)) {
            player->TakeDamage(bullet->GetAttack());
            bullet->OnCollision();
        }
    }
}

/// プレイヤー本体とボス本体の当たり判定を行う。
void CollisionManager::CheckPlayerBossBody(Player* player, Boss1* boss) {
    OBB playerOBB = player->GetOBB();
    OBB bossOBB = boss->GetOBB();
    if (Math::IsCollisionOBB(playerOBB, bossOBB)) {
        player->TakeDamage(boss->GetAttack());
    }
}