#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"

class Player;

/// <summary>
/// 敵クラスの基底クラス。
/// 共通処理（初期化／更新／描画／デバッグなど）を定義する。
/// 派生クラスで個別の移動・攻撃ロジックを実装する。
/// </summary>
class EnemyBase
{
public:
    EnemyBase() = default;
    virtual ~EnemyBase() = default;

    /// <summary>
    /// 初期化処理（モデル・位置など）。
    /// 派生クラスでは独自モデルや初期値を設定する。
    /// </summary>
    virtual void Initialize(Object3dBase* object3dBase);

    /// <summary>
    /// 毎フレーム更新。
    /// （死亡チェック・移動・攻撃・弾更新など）
    /// </summary>
    virtual void Update();

    /// <summary>
    /// 描画処理。
    /// </summary>
    virtual void Draw();

    /// <summary>
    /// デバッグ情報の描画（ImGuiなど）。
    /// </summary>
    virtual void Debug(int id);

    /// <summary>
    /// 衝突時のコールバック。
    /// </summary>
    virtual void OnCollision();

    /// <summary>
    /// ダメージを受けた際の処理。
    /// </summary>
    virtual void TakeDamage(int damage);

    /// <summary>
    /// 現在のOBBを返す。
    /// </summary>
    virtual OBB GetOBB() const;

public: 
    bool IsDead() const { return isDead_; }
    bool IsDeathParticle() const { return isDeathParticle_; }
    const Vector3& GetPosition() const { return transform_.translate; }
    float GetRadius() const { return radius_; }

    void SetScale(const Vector3& scale) { transform_.scale = scale; }
    void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
    void SetTranslate(const Vector3& position) { transform_.translate = position; }
    void SetPlayer(Player* player) { player_ = player; }
    void SetIsDeathParticle(bool flag) { isDeathParticle_ = flag; }

protected:
    // 共通メンバ変数
    Object3dBase* object3dBase_ = nullptr;
    std::unique_ptr<Object3d> object_;
    Transform transform_;
    Player* player_ = nullptr;

    Vector4 color_ = { 1, 1, 1, 1 };
    bool isDead_ = false;
    bool isDeathParticle_ = false;
    int hp_ = 100;

    float radius_ = 1.0f;
    float damageColorTimer_ = 0.0f;
    const float damageColorDuration_ = 0.1f;

    const float DeltaTime_ = 1.0f / 60.0f;
};
