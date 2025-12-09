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
    /// - 引数の <c>Object3dBase*</c> を保持し、内部の <c>Object3d</c> を生成・初期化する想定。
    /// - モデルや初期トランスフォームの設定を行う実装を想定する。
    /// </summary>
    void Initialize(MyEngine::Object3dBase* object3d_base);

    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// - 速度に基づく位置更新、寿命タイマーのデクリメント、衝突判定やデス処理の判定を行う。
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理を行う。
    /// - 内部の <c>Object3d</c> に対して Transform を反映し、描画コマンドを発行する。
    /// </summary>
    void Draw();

    /// <summary>
    /// 衝突時のコールバック。
    /// - 当たり判定にヒットした際に呼び出され、デスフラグやデス処理を開始する。
    /// </summary>
    void OnCollision();

    /// <summary>
    /// OBB（Oriented Bounding Box）を取得する。
    /// - 現在の位置・回転・スケールと内部寸法から OBB を構築して返す。
    /// </summary>
    /// <returns>計算された OBB。</returns>
    OBB GetOBB() const;

    /// <summary>
    /// デスフラグが立っているかを取得する。
    /// </summary>
    /// <returns>デッドなら true。</returns>
    bool IsDead() const { return is_dead_; }

public:
    /// Getter ///

    /// <summary>
    /// 現在の座標（Transform.translate）を取得する。
    /// </summary>
    /// <returns>弾のワールド座標の const 参照。</returns>
    const Vector3& GetTranslate() const { return transform_.translate; }

    /// <summary>
    /// 衝突判定等に使用する半径を取得する。
    /// </summary>
    /// <returns>弾の半径。</returns>
    float GetRadius() const { return radius_; }

    /// <summary>
    /// 弾の攻撃力を取得する。
    /// </summary>
    /// <returns>攻撃力（ダメージ量）。</returns>
    int GetAttack() const { return attack_; }

    /// Setter ///

    /// <summary>
    /// 座標を設定する（変名: GetTranslate）。
    /// - 引数の位置を内部の Transform.translate に設定する。
    /// </summary>
    /// <param name="position">設定するワールド座標。</param>
    void SetTranslate(const Vector3& position) { transform_.translate = position; }

    /// <summary>
    /// 速度を設定する。
    /// - Update() 内でこの速度に基づいて位置を更新する想定。
    /// </summary>
    /// <param name="velocity">設定する速度ベクトル。</param>
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
