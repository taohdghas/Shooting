#include "MyGame.h"

//初期化
void MyGame::Initialize() {
	D3DResourceLeakChecker leakCheck;

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
}

//終了
void MyGame::Finalize() {

}

//毎フレーム更新
void MyGame::Update() {

}

//描画
void MyGame::Draw() {

}