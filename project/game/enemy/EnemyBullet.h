#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "Transform.h"

//敵の弾
/// <summary>
/// 敵キャラクターが発射する弾の状態管理
/// 弾の初期化、毎フレームの移動・更新・寿命管理
/// 弾の描画処理および色・見た目の制御
/// プレイヤーや他オブジェクトとの当たり判定・衝突時処理
/// OBBや座標・攻撃力等の取得・設定インターフェースの提供
/// デスフラグや寿命タイマーによる弾の自動消滅制御
/// </summary>
class EnemyBullet
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
    /// 衝突時のコールバック
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
    /// Getter ///

    /// <summary>
    /// 現在の座標を取得
    /// <returns>現在の座標（Vector3参照）</returns>
    /// </summary>
    const Vector3& GetTranslate() const { return transform_.translate; }

    /// <summary>
    /// 衝突判定等に使用する半径を取得
    /// <returns>半径</returns>
    /// </summary>
    float GetRadius() const { return radius_; }

    /// <summary>
    /// 弾の攻撃力を取得
    /// <returns>攻撃力</returns>
    /// </summary>
    int GetAttack() const { return attack_; }

    /// Setter ///

    /// <summary>
    /// 座標を設定。
    /// <param name="position">設定する座標値</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetTranslate(const Vector3& position) { transform_.translate = position; }

    /// <summary>
    /// 速度を設定
    /// <param name="velocity">設定する速度値</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

    /// <summary>
    /// 色を設定
    /// <param name="color">設定する色（Vector4）</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetColor(const Vector4& color) { object_->SetColor(color); }

private:
    MyEngine::Object3dBase* object3d_base_ = nullptr;
    std::unique_ptr< MyEngine::Object3d> object_;
    Transform transform_ = {0.0f,0.0f,0.0f};
    //速度
    Vector3 velocity_ = {0.0f,0.0f,0.0f};
    //モデルの寸法
	Vector3 dimensions_ = { 0.2f,0.2f,0.2f };
    //半径
    float radius_ = 0.1f;
    //攻撃力
    uint32_t attack_ = 5;
    //デスフラグ
    bool is_dead_ = false;
    //デスタイマー
    int death_timer_ = kLifeTime;
    //生存時間
    static const int kLifeTime = 60 * 5;
};
