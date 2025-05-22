#include "GameScene.h"
#include "ImGuiManager.h"


//初期化
void GameScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	//プレイヤー
	player = std::make_unique<Player>();
	playerObject = new Object3d();
	playerObject->Initialize(Object3dBase::GetInstance());
	playerObject->SetModel("plane.obj");
	playerObject->SetScale(Vector3{ 0.2f,0.2f,0.2f });
	player->Initialize(playerObject);

	//天球
	skydome = std::make_unique<Skydome>();
	skydomeObject = new Object3d();
	skydomeObject->Initialize(Object3dBase::GetInstance());
	//skydomeObject->SetModel("skydome.obj");
	skydome->Initialize(skydomeObject);
}

//終了
void GameScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();

	delete playerObject;
	delete skydomeObject;
}

//更新
void GameScene::Update() {
	//プレイヤー
	player->Update();

	//天球
	//skydome->Update();
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//プレイヤー
	player->Draw();

	//天球
	//skydome->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}