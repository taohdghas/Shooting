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
	/// <summary>
	/// プレイヤーと敵（および各々の弾）間の衝突判定を行う。
	/// - player の弾一覧と enemy を比較してヒットがあれば enemy->TakeDamage() と弾の OnCollision() を呼ぶ。
	/// - enemy の弾一覧と player を比較してヒットがあれば player->TakeDamage() と弾の OnCollision() を呼ぶ。
	/// - 最後にプレイヤー本体と敵本体の当たり判定を行い、衝突があれば双方の衝突コールバックを呼ぶ。
	/// - 引数が null の場合は何もしない。
	/// </summary>
	/// <param name="player">判定対象の Player オブジェクト（ポインタ）。</param>
	/// <param name="enemy">判定対象の Enemy オブジェクト（ポインタ）。</param>
	void CheckPECollisions(Player*player,Enemy*enemy);
private:

};

