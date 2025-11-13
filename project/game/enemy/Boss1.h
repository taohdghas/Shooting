#pragma once
#include "EnemyBase.h"

/// <summary>
/// ステージ1のボス用クラス
/// </summary>
class Boss1 : public EnemyBase
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(Object3dBase* object3dBase) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw() override;

    /// <summary>
    /// デバッグ表示
    /// </summary>
    void Debug();

    /// <summary>
    /// OBB（当たり判定）取得
    /// </summary>
    OBB GetOBB() const override;

private:
  
};
