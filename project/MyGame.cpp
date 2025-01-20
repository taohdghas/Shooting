#include "MyGame.h"

//初期化
void MyGame::Initialize() {

	//基底クラスの初期化
	Framework::Initialize();

	//Sprite初期化
	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteBase, "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(sprite);
	}
	sprites[0]->Initialize(spriteBase, "resources/uvChecker.png");

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


	//object3ds[0]->SetCamera(camera);
	//object3ds[1]->SetCamera(camera);

	//Vector3 cameraRote = camera->GetRotate();
	//Vector3 cameraPos = camera->GetTranslate();
	//object3dBase->SetDefaultCamera(camera);

	//パーティクルエミッター
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();
}

//終了
void MyGame::Finalize() {
	//CloseHandle(fenceEvent);

	//パーティクルエミッター
	delete particleEmitter;
	//オブジェクト
	for (Object3d* object3d : object3ds) {
		delete object3d;
	}
	//Sprite
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	//基底クラスの終了
	Framework::Finalize();
}

//毎フレーム更新
void MyGame::Update() {
	
	//基底クラスの更新
	Framework::Update();

	//ImGui開始
	imguimanager->Begin();

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

	//ImGui描画
	imguimanager->Draw();

	//描画後処理
	directxBase->PostDraw();
}
