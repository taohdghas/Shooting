#include "GameScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"


//初期化
void GameScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");
	ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	ModelManager::GetInstance()->LoadModel("enemy/enemybullet.obj");
	ModelManager::GetInstance()->LoadModel("skydome/skydome.obj");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0,0,-10 });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//天球
	skydome = std::make_unique<Skydome>();
	skydome->Initialize(Object3dBase::GetInstance());

	//パーティクル
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/uvChecker.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/circle2.png", ParticleType::Normal);
	ParticleManager::GetInstance()->CreateparticleGroup("particle3", "resources/gradationLine.png", ParticleType::Ring);
	ParticleManager::GetInstance()->CreateparticleGroup("particle4", "resources/gradationLine.png", ParticleType::Cylinder);
	ParticleManager::GetInstance()->CreateparticleGroup("particle5", "resources/circle2.png", ParticleType::Explosive);

	//衝突マネージャー
	collisionManager = std::make_unique<CollisionManager>();

	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");
    
	for (auto& playerData : levelData->players) {
		player = std::make_unique<Player>();
		player->Initialize(Object3dBase::GetInstance());
		Transform transform;
	
		transform.translate = playerData.translation;
		
		player->SetTranslate(transform.translate);
	}

	for (auto& enemyData : levelData->enemies) {
		auto newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(Object3dBase::GetInstance());
		newEnemy->SetTranslate(enemyData.translation);
		newEnemy->SetPlayer(player.get()); 
		enemies.push_back(std::move(newEnemy));
	}

	//最初の1フレーム入力を無視
	Input::GetInstance()->ClearInput();
}

//終了
void GameScene::Finalize() {
	//パーティクルグループの開放a
	ParticleManager::GetInstance()->Clear();
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void GameScene::Update() {
	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();

	//プレイヤー
	player->Update();

	//敵
	for (auto& enemy : enemies) {
		enemy->Update();
	}

	//天球
	skydome->Update();

	//衝突チェック
	//collisionManager->CheckPECollisions(player.get(), enemy.get());

	
	
	
	//パーティクル
	ParticleManager::GetInstance()->Update();
	for (auto& particle : particleEmitter) {
		particle->Update();
	}

	//タイトルシーンへ
	if (player->IsDead()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}

	//クリアシーンへ
	if (Input::GetInstance()->PushKey(DIK_C)) {
		SceneManager::GetInstance()->ChangeScene("CLEAR");
	}

	//デバック
	Debug();
}

//描画
void GameScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//プレイヤー
	player->Draw();

	//敵
	for (auto& enemy : enemies) {
		enemy->Draw();
	}

	//天球
	//skydome->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//パーティクル
	ParticleManager::GetInstance()->Draw();
}

//デバック
void GameScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	//カメラ
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	//プレイヤーDebug
	player->Debug();
	//敵Debug
	//enemy->Debug();

	ImGui::End();
#endif
}