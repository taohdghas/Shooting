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


	//JsonManager
	jsonManager = std::make_unique<JsonManager>();
    levelData = jsonManager->LoadJsonFile("untitled");

	for (auto& objectData : levelData->objects) {

		std::string name = objectData.fileName + ".obj";

		ModelManager::GetInstance()->LoadModel(name);

		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dBase::GetInstance());
		object3d->SetModel(name);
		object3d->SetScale(objectData.scaling);
		object3d->SetRotate(objectData.rotation);
		object3d->SetTranslate(objectData.translation);
		object3ds.push_back(std::move(object3d));
	}
	
	//プレイヤー
	player_ = std::make_unique<player>();
	player_->Initialize(Object3dBase::GetInstance());
	//プレイヤー配置データからプレイヤーを配置
	if (!levelData->players.empty()) {
		auto& playerData = levelData->players[0];
		player_->SetPosition(playerData.translation);
		player_->SetRotate(playerData.rotation);
	}
}

//終了
void GameScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {

	for (auto& object : object3ds) {
		object->Update();
	}

	player_->Update();
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	for (auto& object : object3ds) {
		object->Draw();
	}

	player_->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}