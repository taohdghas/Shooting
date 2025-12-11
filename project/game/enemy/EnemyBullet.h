#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"

//敵の弾
class EnemyBullet
{
public:
    /// <summary>
    /// 初期化する。
    /// </summary>
    void Initialize(MyEngine::Object3dBase* object3d_base);

    /// <summary>
    /// 毎フレームの更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理を行う
    /// </summary>
    void Draw();

    /// <summary>
    /// 衝突時のコールバック
    /// </summary>
    void OnCollision();

    /// <summary>
    /// OBB（Oriented Bounding Box）を取得する
    /// </summary>
    OBB GetOBB() const;

    /// <summary>
    /// デスフラグが立っているかを取得する。
    /// </summary>
    bool IsDead() const { return is_dead_; }

public:
    /// Getter ///

    /// <summary>
    /// 現在の座標（Transform.translate）を取得する。
    /// </summary>
    const Vector3& GetTranslate() const { return transform_.translate; }

    /// <summary>
    /// 衝突判定等に使用する半径を取得する。
    /// </summary>
    float GetRadius() const { return radius_; }

    /// <summary>
    /// 弾の攻撃力を取得する。
    /// </summary>
    int GetAttack() const { return attack_; }

    /// Setter ///

    /// <summary>
    /// 座標を設定する（変名: GetTranslate）。
    /// </summary>
    void SetTranslate(const Vector3& position) { transform_.translate = position; }

    /// <summary>
    /// 速度を設定する。
    /// </summary>
    void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private:
    MyEngine::Object3dBase* object3d_base_ = nullptr;
    std::unique_ptr< MyEngine::Object3d> object_;
    Transform transform_;
    //速度
    Vector3 velocity_;
    //半径
    float radius_ = 0.1f;
    //モデルの寸法
    float dimensions_ = 0.2f;
    //攻撃力
    uint32_t attack_ = 5;
    //デスフラグ
    bool is_dead_ = false;
    //デスタイマー
    int death_timer_ = kLifeTime;
    //生存時間
    static const int kLifeTime = 60 * 5;
};
