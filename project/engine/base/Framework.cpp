#include "Framework.h"

//初期化
void Framework::Initialize() {
	//WindowsAPIの初期化
	windowsAPI_ = std::make_unique<WindowsAPI>();
	windowsAPI_->Initialize();

	//DirectXの初期化
	directxBase_ = std::make_unique<DirectXBase>();
	directxBase_->Initialize(windowsAPI_.get());

	//入力の初期化
	Input::GetInstance()->Initialize(windowsAPI_.get());

	//SpriteBaseの初期化
	SpriteBase::GetInstance()->Initialize(directxBase_.get());

	//srvManagerの初期化
	SrvManager::GetInstance()->Initialize(directxBase_.get());

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(directxBase_.get(), SrvManager::GetInstance());

	//初期化
	Object3dBase::GetInstance()->Initialize(directxBase_.get());

	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase_.get());

	//ImGuiマネージャ
	imguimanager_ = std::make_unique<ImGuiManager>();
	imguimanager_->Initialize(windowsAPI_.get(), directxBase_.get(), SrvManager::GetInstance());

	//カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetRotate({ 0.3f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,4.0f,-10.0f });
	Object3dBase::GetInstance()->SetDefaultCamera(camera_.get());
	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(directxBase_.get(), SrvManager::GetInstance(), camera_.get());

	//シーンマネージャの生成
	sceneManager = SceneManager::GetInstance();
}

//終了
void Framework::Finalize() {
	//シーンマネージャ
	sceneManager->Finalize();
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//ImGui
	imguimanager_->Finalize();
	//object3dbase
	Object3dBase::GetInstance()->Finalize();
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	//srvマネージャ終了
	SrvManager::GetInstance()->Finalize();
	//SpriteBase
	SpriteBase::GetInstance()->Finalize();
	//入力解放
	Input::GetInstance()->Finalize();
	//windowsAPI
	windowsAPI_->Finalize();
}

//毎フレーム更新
void Framework::Update() {

	// Windowにメッセージが来てたら最優先で処理させる
	if (windowsAPI_->ProcessMessage()) {
		//ゲームループを抜ける
		endRequst_ = true;
	}

	//入力の更新
	Input::GetInstance()->Update();

	//カメラの更新
	camera_->Update();

	//シーンマネージャの更新
	sceneManager->Update();
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