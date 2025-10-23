#pragma once
#include "Struct.h"
#include <d3d12.h>
#include <wrl.h>

/// <summary>
/// ワールドトランスフォームのクラス
/// </summary>
class WorldTransform
{
public:
    /// <summary>
    /// 初期化
    /// </summary>
    /// <remarks>
    /// 現在は特に処理を行わない。必要に応じて初期化処理を追加可能。
    /// </remarks>
    void Initialize();

    /// <summary>
    /// 行列計算
    /// </summary>
    /// <remarks>
    /// ローカルスケール、ローカル回転、ローカル平行移動をもとに
    /// ローカル→ワールド変換行列を計算する。
    /// 親トランスフォームが設定されている場合は親のワールド行列を掛ける。
    /// </remarks>
    void UpdateMatrix();

public:
    //ローカルスケール
    Vector3 scale_ = { 1,1,1 };
    //X,Y,Z軸回りのローカル回転角
    Vector3 rotation_ = { 0,0,0 };
    //ローカル座標
    Vector3 translation_ = { 0,0,0 };
    //ローカル→ワールド変換行列
    Matrix4x4 matWorld_;
    //親となるワールド変換へのポインタ
    const WorldTransform* parent_ = nullptr;

private:

};
