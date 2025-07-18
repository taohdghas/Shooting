#include "GameScene.h"
#include "CameraManager.h"

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

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

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
	auto playerObject = std::make_unique<Object3d>();
	playerObject->Initialize(Object3dBase::GetInstance());
	for (auto& playerData : levelData->players) {

		//
		//playerObject->SetModel("plane.obj");

		playerObject->SetTranslate(playerData.translation);
		playerObject->SetRotate(playerData.rotation);
		playerObject->SetScale(playerData.scaling);

		playerObjects.push_back(std::move(playerObject));
	}
}

//終了
void GameScene::Finalize() {
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {

	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();

	for (auto& object : object3ds) {
		object->Update();
	}
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	
	for (auto& object : object3ds) {
		object->Draw();
	}
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}