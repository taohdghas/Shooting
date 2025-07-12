#include "Framework.h"
#include "CameraManager.h"

//初期化
void Framework::Initialize() {
	//WindowsAPIの初期化
	windowsAPI_ = std::make_unique<WindowsAPI>();
	windowsAPI_->Initialize();

	//DirectXの初期化
	DirectXBase::GetInstance()->Initialize(windowsAPI_.get());

	//入力の初期化
	Input::GetInstance()->Initialize(windowsAPI_.get());

	//SpriteBaseの初期化
	SpriteBase::GetInstance()->Initialize(DirectXBase::GetInstance());

	//srvManagerの初期化
	SrvManager::GetInstance()->Initialize(DirectXBase::GetInstance());

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(DirectXBase::GetInstance(), SrvManager::GetInstance());

	//初期化
	Object3dBase::GetInstance()->Initialize(DirectXBase::GetInstance());

	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(DirectXBase::GetInstance());

	//ImGuiマネージャ
	imguimanager_ = std::make_unique<ImGuiManager>();
	imguimanager_->Initialize(windowsAPI_.get(), DirectXBase::GetInstance(), SrvManager::GetInstance());

	//カメラ
	CameraManager::GetInstance()->Initialize();

	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(DirectXBase::GetInstance(), SrvManager::GetInstance(), camera_.get());

	//シーンマネージャの生成
	sceneManager = SceneManager::GetInstance();
}

//終了
void Framework::Finalize() {
	//シーンマネージャ
	sceneManager->Finalize();
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//カメラ
	CameraManager::GetInstance()->Finalize();
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
	DirectXBase::GetInstance()->Finalize();
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