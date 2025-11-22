#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include <memory>

//プレイヤーの弾のクラス
class PlayerBullet
{
public:
	/// <summary>
	/// 初期化する。
	/// - 引数の <c>Object3dBase*</c> を保持し、内部で <c>Object3d</c> を生成・初期化する。
	/// - モデルは "player/playerbullet.obj" を設定し、スケール等の初期値を与える。
	/// - 寿命タイマー（death_timer_）を <c>kLifeTime</c> にセットする。
	/// </summary>
	void Initialize(Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレーム更新する。
	/// - デスフラグが立っていれば処理を行わない。
	/// - 速度に従い位置を更新し、寿命タイマーをデクリメントして寿命切れならデッドにする。
	/// - 内部の <c>Object3d</c> に対して位置を反映し、<c>Update()</c> を呼ぶ。
	/// </summary>
	void Update();

	/// <summary>
	/// 描画を行う。
	/// - デスフラグが立っている場合は描画を行わない。
	/// - それ以外では内部の <c>Object3d::Draw()</c> を呼ぶ。
	/// </summary>
	void Draw();
	/// <summary>
	/// 衝突時コールバック。
	/// - 衝突を受けた際に呼び出され、デスフラグを立てる等の処理を行う。
	/// </summary>
	void OnCollision();
	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する。
	/// - 現在のトランスフォーム（位置・回転・スケール）と内部寸法から OBB を構築して返す。
	/// </summary>
	/// <returns>計算された OBB。</returns>
	OBB GetOBB() const;
	/// <summary>
	/// デスフラグが立っているかを取得する。
	/// </summary>
	/// <returns>デッドなら true を返す。</returns>
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
	/// - Update() でこの速度を用いて位置が更新される。
	/// </summary>
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
private:
	std::unique_ptr<Object3d> object_;
	Object3dBase* object3d_base_;
	Transform transform_;
	//速度
	Vector3 velocity_;
	//寿命
	static const uint32_t kLifeTime = 60 * 5;
	//デスタイマー
	int death_timer_;
	//デスフラグ
	bool is_dead_ = false;
	float radius_ = 0.1f;
	//モデルの寸法
	float dimensions_ = 0.2f;
	uint32_t attack_ = 5;
};
