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
	//初期化
	void Initialize()override;
	//終了
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
	//デバック
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
	//撃破演出Y移動
	float deathVelocityY = 0.25f;           
	//撃破演出回転速度
	float deathRotationSpeed = 5.0f;  
	//重力加速度
	float gravity = 0.015f;
};

