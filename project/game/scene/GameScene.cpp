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
	ModelManager::GetInstance()->LoadModel("platform/platform.obj");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0,0,-10 });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialize(Object3dBase::GetInstance());
	//敵
	enemy = std::make_unique<Enemy>();
	enemy->Initialize(Object3dBase::GetInstance());
	enemy->SetPlayer(player.get());
	//skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//プラットフォーム
	platform = std::make_unique<Platform>();
	platform->Initialize(Object3dBase::GetInstance());

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
	enemy->Update();
	//skybox
	skybox->Update();
	//プラットフォーム
	platform->Update();
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
    enemy->Draw();
	//天球
	skybox->Draw();
	//プラットフォーム
	platform->Draw();
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
	enemy->Debug();
	//プラットフォームDebug
	platform->Debug();

	ImGui::End();
#endif
}