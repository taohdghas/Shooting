#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include <memory>

//プレイヤーの弾のクラス
/// <summary>
/// プレイヤーが発射する弾の状態管理
/// 弾の初期化、毎フレームの移動・更新・寿命管理
/// 弾の描画処理および見た目の制御
/// 敵や他オブジェクトとの当たり判定・衝突時処理
/// OBBや座標・攻撃力等の取得・設定インターフェースの提供
/// デスフラグや寿命タイマーによる弾の自動消滅制御
/// </summary>
class PlayerBullet
{
public:
	/// <summary>
	/// 初期化
	/// <param name="object3d_base">3Dオブジェクト共通設定へのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 更新
	/// <returns>なし</returns>
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突時コールバック
	/// <returns>なし</returns>
	/// </summary>
	void OnCollision();

	/// <summary>
	/// OBBを取得
	/// <returns>OBB構造体</returns>
	/// </summary>
	OBB GetOBB() const;

	/// <summary>
	/// デスフラグが立っているかを取得
	/// <returns>デスフラグが立っていればtrue</returns>
	/// </summary>
	bool IsDead() const { return is_dead_; }
public:
	///Getter///
	/// <summary>
	/// 現在の位置を取得
	/// <returns>現在の座標（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetPosition() const { return transform_.translate; }

	/// <summary>
	/// 衝突判定等に用いる半径を取得
	/// <returns>半径</returns>
	/// </summary>
	float GetRadius() const { return radius_; }

	/// <summary>
	/// 弾の攻撃力を取得
	/// <returns>攻撃力</returns>
	/// </summary>
	int GetAttack() const { return attack_; }

	///Setter///
	/// <summary>
	/// 弾の位置を設定
	/// <param name="position">設定する座標値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetPosition(const Vector3& position) { transform_.translate = position; }

	/// <summary>
	/// 弾の速度を設定
	/// <param name="velocity">設定する速度値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
private:
	std::unique_ptr< MyEngine::Object3d> object_;
	MyEngine::Object3dBase* object3d_base_;
	Transform transform_;
	//速度
	Vector3 velocity_;
	//モデルの寸法
	Vector3 dimensions_ = { 0.2f,0.2f,0.2f };
	//寿命
	static const uint32_t kLifeTime = 60 * 5;
	//デスタイマー
	int death_timer_;
	//デスフラグ
	bool is_dead_ = false;
	//半径
	float radius_ = 0.1f;
	uint32_t attack_ = 5;
};