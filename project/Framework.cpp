#include "Framework.h"

//初期化
void Framework::Initialize() {
	//WindowsAPIの初期化
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();

	//DirectXの初期化
	directxBase = new DirectXBase();
	directxBase->Initialize(windowsAPI);

	//入力の初期化
	input = new Input();
	input->Initialize(windowsAPI);

	//SpriteBaseの初期化
	spriteBase = new SpriteBase;
	spriteBase->Initialize(directxBase);

	//srvManagerの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(directxBase);

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(directxBase, srvManager);
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//初期化
	object3dBase = new Object3dBase();
	object3dBase->Initialize(directxBase);

	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	//ImGuiマネージャ
	imguimanager->Initialize(windowsAPI, directxBase, srvManager);

	//サウンド
	audio_ = Audio::GetInstance();
	audio_->Initialize();

	//カメラ
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });

	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(directxBase, srvManager, camera);
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/circle.png");
}

//終了
void Framework::Finalize() {
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//カメラ
	delete camera;
	//Audio
	audio_->Finalize();
	//ImGui
	imguimanager->Finalize();
	//object3dbase
	delete object3dBase;
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	//srvマネージャ終了
	delete srvManager;
	//SpriteBase
	delete spriteBase;
	//入力解放
	delete input;
	//DirectX解放
	delete directxBase;
	//WindowsAPIの終了処理
	windowsAPI->Finalize();
}

//毎フレーム更新
void Framework::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (windowsAPI->ProcessMessage()) {
		//ゲームループを抜ける
		endRequst_ = true;
	}

	//入力の更新
	input->Update();

	//カメラの更新
	camera->Update();

}

//実行
void Framework::Run(){
	//ゲームの初期化
	Initialize();

	//ゲームループ
	while (true) {
		//毎フレーム更新
		Update();
		//終了リクエストで抜ける
		if (IsEndRequst()) {
			break;
		}
		//描画
		Draw();
	}
	//終了
	Finalize();
}