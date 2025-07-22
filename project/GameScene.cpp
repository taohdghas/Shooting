#include "GameScene.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

//初期化
void GameScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("player.obj");
	ModelManager::GetInstance()->LoadModel("enemy.obj");
	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-30.0f });
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
	for (auto& playerData : levelData->players) {
		auto playerObject = std::make_unique<Object3d>();
		playerObject->Initialize(Object3dBase::GetInstance());
		playerObject->SetModel("player.obj");
		playerObject->SetTranslate(playerData.translation);
		playerObject->SetRotate(playerData.rotation);
		playerObject->SetScale(playerData.scaling);

		playerObjects.push_back(std::move(playerObject));
	}
	//敵
	for (auto& enemyData : levelData->enemies) {
		//生成
		auto enemyObject = std::make_unique<Object3d>();
		//初期化
		enemyObject->Initialize(Object3dBase::GetInstance());
		enemyObject->SetModel("enemy.obj");
		enemyObject->SetTranslate(enemyData.translation);
		enemyObject->SetRotate(enemyData.rotation);
		enemyObject->SetScale(enemyData.scaling);
		enemyObjects.push_back(std::move(enemyObject));
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

	for (auto& object : playerObjects) {
		object->Update();
	}
	for (auto& enemyObject : enemyObjects) {
		enemyObject->Update();
	}

#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRotate = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRotate.x,cameraRotate.y,cameraRotate.z });
		ImGui::TreePop();
	}
	ImGui::End();
#endif
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	
	for (auto& object : playerObjects) {
		object->Draw();
	}
	for (auto& enemyObject : enemyObjects) {
		enemyObject->Draw();
	}
	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();
}