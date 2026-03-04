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
/// <summary>
/// プレイヤー、敵、ボス、各種弾同士の衝突判定処理の一元管理
/// プレイヤーと敵、プレイヤーとボス、各種弾とキャラクター間の当たり判定ロジックの実装
/// 衝突時のコールバック呼び出しやダメージ処理のトリガー
/// ゲーム内の主要キャラクター・弾の相互作用を安全かつ効率的に制御
/// 衝突判定のためのインターフェース提供
/// </summary>
class CollisionManager {
public:
	/// <summary>
	/// プレイヤーと敵の衝突判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckPlayerEnemyCollisions(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー弾と敵の当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckPlayerBulletsToEnemy(Player* player, Enemy* enemy);

	/// <summary>
	/// 敵弾とプレイヤーの当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckEnemyBulletsToPlayer(Player* player, Enemy* enemy);

	/// <summary>
	/// プレイヤー本体と敵本体の当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="enemy">敵キャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckPlayerEnemyBody(Player* player, Enemy* enemy);

	/// <summary>
    /// プレイヤーとボスと各弾の衝突判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="boss">ボスキャラクターのポインタ</param>
	/// <returns>なし</returns>
    /// </summary>
	void CheckPlayerBossCollisions(Player* player, Boss1* boss);

	/// <summary>
	/// プレイヤー弾とボスの当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="boss">ボスキャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckPlayerBulletsToBoss(Player* player, Boss1* boss);

	/// <summary>
	/// ボス弾とプレイヤーの当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="boss">ボスキャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckBossBulletsToPlayer(Player* player, Boss1* boss);

	/// <summary>
	/// プレイヤー本体とボス本体の当たり判定を行う
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="boss">ボスキャラクターのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void CheckPlayerBossBody(Player* player, Boss1* boss);

private:
};
