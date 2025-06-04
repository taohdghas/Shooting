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
	ModelManager::GetInstance()->LoadModel("player.obj");
	ModelManager::GetInstance()->LoadModel("playerbullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy.obj");
	ModelManager::GetInstance()->LoadModel("enemybullet.obj");


	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialize(Object3dBase::GetInstance());

	//天球
	skydome = std::make_unique<Skydome>();
	skydomeObject = new Object3d();
	skydomeObject->Initialize(Object3dBase::GetInstance());
	//skydomeObject->SetModel("skydome.obj");
	skydome->Initialize(skydomeObject);

	//最初の1フレーム入力を無視
	Input::GetInstance()->ClearInput();

}

//終了
void GameScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();

	delete skydomeObject;
}

//更新
void GameScene::Update() {
	//プレイヤー
	player->Update();

	//天球
	//skydome->Update();

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

	//天球
	//skydome->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}