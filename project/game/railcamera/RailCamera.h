#pragma once
#include "Camera.h"
#include "Player.h"
#include "Platform.h"

// レールカメラクラス
/// <summary>
/// レールに沿ったカメラの移動制御
/// カメラの初期化・毎フレームの位置更新処理
/// カメラに追従するプレイヤーやプラットフォームの座標制御
/// カメラ・プレイヤー・プラットフォーム間のオフセット管理
/// ゲーム進行に合わせたカメラの自動移動
/// </summary>
class RailCamera
{
public:
    /// <summary>
    /// デフォルトコンストラクタ
    /// <returns>なし</returns>
    /// </summary>
    RailCamera();

    /// <summary>
    /// 初期化
    /// <returns>なし</returns>
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新処理
    /// <returns>なし</returns>
    /// </summary>
    void Update();

    /// <summary>
    /// 追従対象のプレイヤーを設定
    /// <param name="player">追従対象のプレイヤーポインタ</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetPlayer(Player* player) { player_ = player; };

    /// <summary>
    /// プレイヤーに対するオフセットを設定
    /// <param name="offset">プレイヤーに対するオフセット値</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetPlayerOffset(const Vector3& offset) { player_offset_ = offset; };

    /// <summary>
    /// 追従対象のプラットフォームを設定
    /// <param name="platform">追従対象のプラットフォームポインタ</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetPlatform(Platform* platform) { platform_ = platform; };

    /// <summary>
    /// プラットフォームに対するオフセットを設定
    /// <param name="offset">プラットフォームに対するオフセット値</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetPlatformOffset(const Vector3& offset) { platform_offset_ = offset; };

    /// <summary>
    /// カメラ追従速度を設定
    /// <param name="speed">カメラ追従速度</param>
    /// <returns>なし</returns>
    /// </summary>
    void SetSpeed(float speed) { speed_ = speed; };

    /// <summary>
    /// 内部で管理している Camera のポインタを取得
    /// <returns>Cameraクラスのポインタ</returns>
    /// </summary>
    MyEngine::Camera* GetCamera() { return &camera_; };

private:
    MyEngine::Camera camera_;
    float speed_ = 3.0f;
    Player* player_ = nullptr;
    Vector3 player_offset_;
    Platform* platform_ = nullptr;
    Vector3 platform_offset_ = { 0, 0, 0 };
    bool followEnabled_;
};