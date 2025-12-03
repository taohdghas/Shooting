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

#include <vector>

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
	//スタート演出
	void StartAnimation();

	void FollowCamera();
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
	//カメラ
	std::unique_ptr<Camera>camera;
	//JsonManager
	std::unique_ptr<JsonManager>jsonManager;
	//フェード
	std::unique_ptr<Fade>fade;
	//レベルデータ
	LevelData* levelData;
	//1回転にかかる時間
	const float totalRotationTime = 5.0f;
	//角速度
	const float rotationSpeed = 1.0f;    
	//収束に書ける時間
	const float oneRotation = 2.0f * 3.14159f;
	//カメラ回転アニメーションフラグ
	bool isStartAnimation = true;
	//カメラ収束フラグ
	bool isReturning = false;
	bool isFollowingInitialized = false; // 追従開始時フラグ
	//回転タイマー
	float cameraRotateTimer = 0.0f;
	float zOffset = 0.0f;               // プレイヤーとのZオフセット
	//回転開始時のカメラ位置
	Vector3 cameraStartPos;
	//回転開始時のカメラ回転
	Vector3 cameraStartRot;
};

