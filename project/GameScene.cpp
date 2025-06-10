#include "GameScene.h"

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

	//Sprite初期化
	for (uint32_t i = 0; i < 2; ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(std::move(sprite)); 
	}
	sprites[0]->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");

	//3Dオブジェクト
	for (uint32_t i = 0; i < 2; ++i) {
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dBase::GetInstance());
		object3ds.push_back(std::move(object3d));
	}
	object3ds[0]->SetModel("plane.obj");
	object3ds[0]->SetTranslate({ 0.0f,0.0f,0.0f });
	object3ds[1]->SetModel("axis.obj");
	object3ds[1]->SetTranslate({ 2.0f,0.0f,0.0f });

	Vector3 objectrotate = object3ds[0]->GetRotate();

	//JsonManager
	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");
	

}

//終了
void GameScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {

}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}