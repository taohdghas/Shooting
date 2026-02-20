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
    /// デフォルトコンストラクタ。
    /// </summary>
    RailCamera();

    /// <summary>
    /// 初期化処理を行う
    /// </summary>
    void Initialize();
    /// <summary>
    /// 毎フレームの更新処理を行う
    /// </summary>
    void Update();

    /// <summary>
    /// 追従対象のプレイヤーを設定する。
    /// </summary>
	void SetPlayer(Player* player) { player_ = player; };
    /// <summary>
    /// プレイヤーに対するオフセットを設定する。
    /// </summary>
	void SetPlayerOffset(const Vector3& offset) { player_offset_ = offset; };
    /// <summary>
    /// 追従対象のプラットフォームを設定する。
    /// </summary>
	void SetPlatform(Platform* platform) { platform_ = platform; };
    /// <summary>
    /// プラットフォームに対するオフセットを設定する。
    /// </summary>
	void SetPlatformOffset(const Vector3& offset) { platform_offset_ = offset; };
    /// <summary>
    /// カメラ追従速度を設定する。
    /// </summary>
	void SetSpeed(float speed) { speed_ = speed; };

    /// <summary>
    /// 内部で管理している Camera のポインタを取得する。
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
