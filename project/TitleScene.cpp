#include "TitleScene.h"
#include "SceneManager.h"

//初期化
void TitleScene::Initialize() {

	//Sprite初期化
	for (uint32_t i = 0; i < 1; ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(std::move(sprite));
	}
	sprites[0]->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");

	//3Dオブジェクト
	/*
	for (uint32_t i = 0; i < 1; ++i) {
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dBase::GetInstance());
		object3ds.push_back(std::move(object3d));
	}
	object3ds[0]->SetModel("plane.obj");
	object3ds[0]->SetTranslate({ 0.0f,0.0f,0.0f });
	*/
//	Vector3 objectrotate = object3ds[0]->GetRotate();

	object3d = std::make_unique<Object3d>();
	object3d->Initialize(Object3dBase::GetInstance());
	object3d->SetTranslate({ 0.0f,0.0f,0.0f });
	object3d->SetModel("plane.obj");

	//サウンド
	Audio::GetInstance()->Initialize();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/circle.png");
	ParticleManager::GetInstance()->CreateparticleGroup("particle2", "resources/uvChecker.png");
	//パーティクルエミッター
	for (uint32_t i = 0; i < 2; ++i) {
		auto particle = std::make_unique<ParticleEmitter>();
		if (i == 0) {
			particle->Initialize("particle");
		} else
		{

			particle->Initialize("particle2");
		}
		particle->Emit();
		particleEmitter.push_back(std::move(particle));
	}

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	//object3d->SetCamera(camera.get());
}

//終了
void TitleScene::Finalize() {
	ParticleManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void TitleScene::Update() {
	//camera->Update();

	/*
	for (size_t i = 0; i < sprites.size(); ++i) {
		auto& sprite = sprites[i];

		//Spriteの更新
		sprite->Update();
	}
	*/

	//オブジェクト更新
	/*
	for (size_t i = 0; i < object3ds.size(); ++i) {
		auto& object = object3ds[i];

		object->Update();
	}
	*/
	object3d->Update();

	/*
	//パーティクル更新
	ParticleManager::GetInstance()->Update();
	for (size_t i = 0; i < particleEmitter.size(); ++i) {
		auto& particle = particleEmitter[i];
		particle->Update();
	}
	*/
	//エンターキーを押したらゲームシーンへ
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

//描画
void TitleScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	/*
	for (auto& sprite : sprites) {
		//sprite描画処理
		sprite->Draw();
	}
	*/

	//オブジェクト描画
	/*
	for (size_t i = 0; i < object3ds.size(); ++i) {
		auto& object = object3ds[i];

		object->Draw();
	}
	*/
	object3d->Draw();
	//ParticleManager::GetInstance()->Draw();
}