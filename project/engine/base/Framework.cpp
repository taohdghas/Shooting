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
	Input::GetInstance()->Initialize(windowsAPI);

	//SpriteBaseの初期化
	SpriteBase::GetInstance()->Initialize(directxBase);

	//srvManagerの初期化
	srvManager = new SrvManager();
	srvManager->Initialize(directxBase);

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(directxBase, srvManager);

	//初期化
	Object3dBase::GetInstance()->Initialize(directxBase);

	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase);

	//ImGuiマネージャ
	imguimanager->Initialize(windowsAPI, directxBase, srvManager);

	//カメラ
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });

	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(directxBase, srvManager, camera);
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/circle.png");

	//パーティクルエミッター
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();

	//シーンマネージャの生成
	sceneManager_ = SceneManager::GetInstance();
}

//終了
void Framework::Finalize() {
	//シーンファクトリー解放
	delete sceneFactory_;
	//シーンマネージャ
	sceneManager_->Finalize();
	//パーティクルエミッター
	delete particleEmitter;
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//カメラ
	delete camera;
	//ImGui
	imguimanager->Finalize();
	//object3dbase
	Object3dBase::GetInstance()->Finalize();
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	//srvマネージャ終了
	delete srvManager;
	//SpriteBase
	SpriteBase::GetInstance()->Finalize();
	//入力解放
	Input::GetInstance()->Finalize();
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
	Input::GetInstance()->Update();

	//カメラの更新
	camera->Update();

	//シーンマネージャの更新
	sceneManager_->Update();
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