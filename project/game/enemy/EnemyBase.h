#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"

// 前方宣言
class Player;

// 敵の基底クラス
class EnemyBase {
public:
	EnemyBase() = default;
	virtual ~EnemyBase() = default;

	/// <summary>
	/// 初期化する
	/// </summary>
	virtual void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレームの更新処理を行う
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画を行う
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	virtual void OnCollision();

	/// <summary>
	/// HP を減少させる
	/// </summary>
	virtual void TakeDamage(int damage);

	/// <summary>
	/// デバッグ UI を表示する（ImGui）
	/// </summary>
	virtual void Debug(int id);

	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する
	/// </summary>
	virtual OBB GetOBB() const;

	/// <summary>
	/// 死亡フラグ判定
	/// </summary>
	bool IsDead() const { return is_dead_; }

	/// <summary>
	/// 死亡時パーティクル発生フラグ判定
	/// </summary>
	bool IsDeathParticle() const { return is_death_particle_; }

public:
	// ===== Getter =====

	/// <summary>
	/// 現在位置を取得
	/// </summary>
	const Vector3& GetPosition() const { return transform_.translate; }

	/// <summary>
	/// 攻撃力取得
	/// </summary>
	int GetAttack() const { return attack_; }

	/// <summary>
	/// 半径取得（コリジョン・描画用）
	/// </summary>
	float GetRadius() const { return radius_; }

public:
	// ===== Setter（敵ごとの個別設定用）=====

	void SetHP(int hp) { hp_ = hp; }
	void SetAttack(int attack) { attack_ = attack; }
	void SetRadius(float radius) { radius_ = radius; }
	void SetDimensions(float dimensions) { dimensions_ = dimensions; }

	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetTranslate(const Vector3& position) { transform_.translate = position; }

	void SetPlayer(Player* player) { player_ = player; }
	void SetIsDeathParticle(bool flag) { is_death_particle_ = flag; }

protected:
	// ===== 共通リソース =====
	MyEngine::Object3dBase* object3d_base_ = nullptr;
	std::unique_ptr<MyEngine::Object3d> object_;

	Transform transform_;
	Player* player_ = nullptr;

	// ===== ステータス（派生クラスで設定）=====
	int hp_ = 1;
	int attack_ = 1;
	float radius_ = 1.0f;
	float dimensions_ = 1.0f;

	// ===== 状態 =====
	bool is_dead_ = false;
	bool is_death_particle_ = false;

	// ===== ダメージ演出 =====
	Vector3 default_scale_ = { 1.0f, 1.0f, 1.0f };
	float damage_color_timer_ = 0.0f;
	float damage_scale_timer_ = 0.0f;

	// ===== 定数 =====
	const float kDeltaTime = 1.0f / 60.0f;
	const float kDamageColorDuration = 0.1f;
	const float kDamageScaleDuration = 0.08f;
};
