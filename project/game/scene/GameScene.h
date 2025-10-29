#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Audio.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ParticleEmitter.h"
#include "BaseScene.h"
#include "Camera.h"
#include "CollisionManager.h"
#include "JsonManager.h"
#include "Fade.h"
#include "RailCamera.h"

#include <vector>
#include <random>

#include "Player.h"
#include "Enemy.h"
#include "Skybox.h"
#include "Platform.h"

//ゲームシーン
class GameScene : public BaseScene
{
public:
	/// <summary>
	/// シーン初期化処理。
	/// - オーディオの初期化、必要モデルの読み込み、パーティクルグループ作成を行う。
	/// - カメラ（およびレールカメラ）の生成と CameraManager への登録を行い、Object3dBase のデフォルトカメラを設定する。
	/// - Skybox／Platform／CollisionManager 等を初期化し、JsonManager からレベルデータを読み込んでプレイヤー・敵を生成・配置する。
	/// - フェード（FadeIn）を開始し、最初のフレームの入力をクリアする。
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// シーン終了処理。
	/// - ParticleManager のクリア、CameraManager と Audio の終了処理を呼び出してリソースを解放する。
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// 毎フレーム更新処理。
	/// - カメラ（およびレールカメラ）／プレイヤー／敵／Skybox／Platform／パーティクル等を更新する。
	/// - 衝突判定（敵ごとに CollisionManager::CheckPECollisions を呼ぶ）を行い、敵死亡時にデスパーティクルを発生させる。
	/// - プレイヤー死亡や特定キー入力によるシーン遷移判定を行い、フェード更新やデバッグ呼出しを行う。
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画処理。
	/// - Object3d 系の共通描画設定を行い、プレイヤー・敵・Skybox・Platform・パーティクルを描画する。
	/// - Sprite 系の共通描画設定を行った上でプレイヤーのレティクルを描画し、フェードを最後に描画する。
	/// </summary>
	void Draw()override;
	/// <summary>
	/// デバッグ表示処理（ImGui）。
	/// - Camera や Player / Enemy / Platform / Skybox のパラメータを ImGui で調整・表示する。
	/// - 実行中は Update 内から呼び出される。
	/// </summary>
	void Debug()override;
public:
	//ゲームクリアへ
	void ToGameClear();
	//ゲームオーバーへ
	void ToGameOver();
private:
	//プレイヤー
	std::unique_ptr<Player>player;
	//敵
	std::vector<std::unique_ptr<Enemy>>enemies;
	//Skybox
	std::unique_ptr<Skybox>skybox;
	//プラットフォーム
	std::unique_ptr<Platform>platform;
	//パーティクルエミッター
	std::vector<std::unique_ptr<ParticleEmitter>>particleEmitter;
	//衝突マネージャー
	std::unique_ptr<CollisionManager>collisionManager;
	//レールカメラ
	std::unique_ptr<RailCamera>railCamera;
	//カメラ
	std::unique_ptr<Camera>camera;
	//JsonManager
	std::unique_ptr<JsonManager>jsonManager;
	//フェード
	std::unique_ptr<Fade>fade;
	//レベルデータ
	LevelData* levelData;

	//撃破演出回転軸
	Vector3 deathRotationAxis = { 1.0f, 0.0f, 0.0f };
	//撃破演出速度
	Vector3 deathVelocity;
	//乱数生成器
	std::mt19937 randomEngine{ std::random_device{}() };
	std::uniform_real_distribution<float> randomDist{ -1.0f, 1.0f };
	
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;
	//ゲームオーバーシーン遷移フラグ
	bool isToGameOver = false;
	//撃破演出が始まったか
	bool isDeathMotionStarted = false;
	//撃破演出経過時間タイマー
	float deathTimer = 0.0f;                 
	//撃破演出回転速度
	float deathRotationSpeed = 5.0f;  
	//重力加速度
	float gravity = 0.015f;
};

