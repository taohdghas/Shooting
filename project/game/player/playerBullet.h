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
	/// 初期化する
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレーム更新する。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画を行う。
	/// </summary>
	void Draw();
	/// <summary>
	/// 衝突時コールバック。
	/// </summary>
	void OnCollision();
	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する。
	/// </summary>
	OBB GetOBB() const;
	/// <summary>
	/// デスフラグが立っているかを取得する。
	/// </summary>
	bool IsDead() const { return is_dead_; }
public:
	///Getter///
	/// <summary>
	/// 現在の位置（Transform.translate）を取得する。
	/// </summary>
	const Vector3& GetPosition() const { return transform_.translate; }
	/// <summary>
	/// 衝突判定等に用いる半径を取得する。
	/// </summary>
	float GetRadius() const { return radius_; }
	/// <summary>
	/// 弾の攻撃力を取得する。
	/// </summary>
	int GetAttack() const { return attack_; }
	///Setter///
	/// <summary>
	/// 弾の位置を設定する。
	/// </summary>
	void SetPosition(const Vector3& position) { transform_.translate = position; }
	/// <summary>
	/// 弾の速度を設定する。
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