#pragma once
#include "Camera.h"
#include "Platform.h"

class Player;
class RailCamera {
public:
    /// <summary>
    /// デフォルトコンストラクタ。
    /// - 内部のカメラとパラメータを既定値で初期化する。
    /// </summary>
    RailCamera();

    /// <summary>
    /// 初期化処理を行う。
    /// - 必要に応じてカメラの初期設定（FOV/アスペクト比等）や内部状態のリセットを行う。
    /// </summary>
    void Initialize();
    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// - プレイヤーやプラットフォームの位置に基づいてカメラ位置を補間（レール移動）し、内部カメラを更新する。
    /// - speed_ やオフセット値を考慮して追従挙動を決定する。
    /// </summary>
    void Update();

    /// <summary>
    /// 追従対象のプレイヤーを設定する。
    /// - SetPlayer を呼んだあと Update でプレイヤー追従が行われる。
    /// </summary>
    /// <param name="player">追従対象の Player ポインタ。</param>
    void SetPlayer(Player* player);
    /// <summary>
    /// プレイヤーに対するオフセットを設定する。
    /// - カメラがプレイヤーからどの相対位置に配置されるかを指定する。
    /// </summary>
    /// <param name="offset">プレイヤーからの相対オフセットベクトル。</param>
    void SetPlayerOffset(const Vector3& offset);
    /// <summary>
    /// 追従対象のプラットフォームを設定する。
    /// - 必要に応じてプレイヤー追従とプラットフォーム追従を組み合わせてカメラ位置を決定する。
    /// </summary>
    /// <param name="platform">追従対象の Platform ポインタ。</param>
    void SetPlatform(Platform* platform);
    /// <summary>
    /// プラットフォームに対するオフセットを設定する。
    /// - プラットフォーム基準でのカメラ相対位置を指定する。
    /// </summary>
    /// <param name="offset">プラットフォームからの相対オフセットベクトル。</param>
    void SetPlatformOffset(const Vector3& offset);
    /// <summary>
    /// カメラ追従速度を設定する。
    /// - 大きいほど速く追従し、小さいほど緩やかに補間される。
    /// </summary>
    /// <param name="speed">追従速度（任意の単位）。</param>
    void SetSpeed(float speed);

    /// <summary>
    /// 内部で管理している Camera のポインタを取得する。
    /// - 取得した Camera を描画やシーンにセットして利用できる。
    /// </summary>
    /// <returns>内部 Camera へのポインタ。</returns>
    Camera* GetCamera();

private:
    Camera camera_;
    float speed_ = 3.0f;
    Player* player_ = nullptr;
    Vector3 playerOffset_;
    Platform* platform_ = nullptr;
    Vector3 platformOffset_ = { 0,0,0 };
};