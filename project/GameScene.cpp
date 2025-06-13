#include "GameScene.h"
#include "SceneManager.h"
#include "ImGuiManager.h"


//初期化
void GameScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");
	TextureManager::GetInstance()->LoadTexture("resources/player.png");
	TextureManager::GetInstance()->LoadTexture("resources/playerbullet.png");
	TextureManager::GetInstance()->LoadTexture("resources/enemy.png");
	TextureManager::GetInstance()->LoadTexture("resources/enemybullet.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");
	ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemybullet.obj");


	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialize(Object3dBase::GetInstance());

	//敵
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(Object3dBase::GetInstance());

	//天球
	skydome = std::make_unique<Skydome>();


	//パーティクル
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateparticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateparticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);
	
	//衝突マネージャー
	collisionManager = std::make_unique<CollisionManager>();

	//最初の1フレーム入力を無視
	Input::GetInstance()->ClearInput();

}

//終了
void GameScene::Finalize() {
	//パーティクルグループの開放
	ParticleManager::GetInstance()->Clear();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {
	//プレイヤー
	player->Update();

	//敵
	enemy->Update();

	//天球
	//skydome->Update();

	//衝突チェック
	collisionManager->CheckPECollisions(player.get(), enemy.get());

	if (enemy->IsDeathParticle()) {
		auto emitter = std::make_unique<ParticleEmitter>();
		emitter->Initialize("particle3");
		emitter->SetPosition(enemy->GetPosition());
		emitter->Emit(); // 即時発生
		particleEmitter.push_back(std::move(emitter));

		// フラグをリセット
		enemy->SetisDeathParticle(false);
	}

	//パーティクル
	ParticleManager::GetInstance()->Update();
	for (auto& particle : particleEmitter) {
		particle->Update();
	}
	

	//タイトルシーンへ
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//プレイヤー
	player->Draw();

	//敵
	enemy->Draw();

	//天球
	//skydome->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//パーティクル
	ParticleManager::GetInstance()->Draw();
}