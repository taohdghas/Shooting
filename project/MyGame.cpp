#include "MyGame.h"

//初期化
void MyGame::Initialize() {

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

	srvManager = new SrvManager();
	srvManager->Initialize(directxBase);

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(directxBase, srvManager);
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//Sprite初期化
	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteBase, "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(sprite);
	}
	sprites[0]->Initialize(spriteBase, "resources/uvChecker.png");
	//sprites[1]->SettextureSize({ 10.0f,10.0f });
	//sprites[1]->Initialize(spriteBase,"resources/monsterBall.png");

	//初期化
	object3dBase = new Object3dBase();
	object3dBase->Initialize(directxBase);

	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	//3Dオブジェクト
	for (uint32_t i = 0; i < 2; ++i) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dBase);
		object3ds.push_back(object3d);
	}
	object3ds[0]->SetModel("plane.obj");
	object3ds[0]->SetTranslate({ 0.0f,0.0f,0.0f });
	object3ds[1]->SetModel("axis.obj");
	object3ds[1]->SetTranslate({ 2.0f,0.0f,0.0f });

	Vector3 objectrotate = object3ds[0]->GetRotate();

	//カメラ
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	object3ds[0]->SetCamera(camera);
	object3ds[1]->SetCamera(camera);

	Vector3 cameraRote = camera->GetRotate();
	Vector3 cameraPos = camera->GetTranslate();
	object3dBase->SetDefaultCamera(camera);

	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(directxBase, srvManager, camera);
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/circle.png");

	//パーティクルエミッター
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();

	//ImGuiマネージャ
	imguimanager->Initialize(windowsAPI, directxBase, srvManager);

	//サウンド
	audio_ = Audio::GetInstance();
	audio_->Initialize();
	soundData1 = audio_->SoundLoadWave("resources/Alarm01.wav");
}

//終了
void MyGame::Finalize() {
	//CloseHandle(fenceEvent);
	//Audio
	audio_->Finalize();
	audio_->SoundUnload(&soundData1);
	//ImGui
	imguimanager->Finalize();
	//delete imguimanager;
	//パーティクルエミッター
	delete particleEmitter;
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//カメラ
	delete camera;
	//オブジェクト
	for (Object3d* object3d : object3ds) {
		delete object3d;
	}
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	delete object3dBase;
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	delete srvManager;
	//DirectX解放
	delete directxBase;
	//入力解放
	delete input;
	//Sprite
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	//SpriteBase
	delete spriteBase;
	//WindowsAPIの終了処理
	windowsAPI->Finalize();
	//WindowsAPI関数
	delete windowsAPI;
}

//毎フレーム更新
void MyGame::Update() {
	// Windowにメッセージが来てたら最優先で処理させる
	if (windowsAPI->ProcessMessage()) {
		//ゲームループを抜ける
		endRequst_ = true;
	}

	//ImGui開始
	imguimanager->Begin();

	//入力の更新
	input->Update();

	//カメラの更新
	camera->Update();

	//Spriteの更新
	for (size_t i = 0; i < sprites.size(); ++i) {
		Sprite* sprite = sprites[i];

		Vector2 position = sprite->GetPosition();
		//Spriteの更新
		sprite->Update();
#ifdef USE_IMGUI
		ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
		ImGui::Begin(std::format("sprite##{}", i).c_str()); // 一意のIDを付与
		ImGui::DragFloat2(std::format("position##{}", i).c_str(), &position.x, 1.0f);
		ImGui::End();
#endif
		sprite->SetPosition(position);
	}

	//3Dオブジェクトの更新
	for (size_t i = 0; i < object3ds.size(); ++i) {
		Object3d* object3d = object3ds[i];
		object3d->Update();
	}

	//パーティクル更新
//	particleEmitter->Update();
	//ParticleManager::GetInstance()->Update();

	//サウンド再生
	if (input->TriggerKey(DIK_SPACE)) {
		audio_->SoundPlayWave(soundData1);
	}

	//ImGui終了
	imguimanager->End();
}

//描画
void MyGame::Draw() {
	//描画前処理
	directxBase->PreDraw();

	srvManager->PreDraw();

	//3Dオブジェクト描画準備
	object3dBase->DrawBaseSet();

	//共通描画設定
	spriteBase->DrawBaseSet();

	//スプライト描画 
	for (Sprite* sprite : sprites) {
		//sprite描画処理
		sprite->Draw();
	}

	/*
	//3Dオブジェクト描画
	for (Object3d* object3d : object3ds) {
		object3d->Draw();
	}
	*/
	//パーティクル描画
	//ParticleManager::GetInstance()->Draw();

	//ImGui描画
	imguimanager->Draw();

	//描画後処理
	directxBase->PostDraw();
}
