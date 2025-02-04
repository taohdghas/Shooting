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
	for (uint32_t i = 0; i < 2; ++i) {
		auto object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dBase::GetInstance());
		object3ds.push_back(std::move(object3d));
	}
	object3ds[0]->SetModel("plane.obj");
	object3ds[0]->SetTranslate({ 0.0f,0.0f,0.0f });
	object3ds[1]->SetModel("axis.obj");
	object3ds[1]->SetTranslate({ 2.0f,0.0f,0.0f });

	Vector3 objectrotate = object3ds[0]->GetRotate();

	//サウンド
	Audio::GetInstance()->Initialize();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");
}

//終了
void TitleScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void TitleScene::Update() {
	for (size_t i = 0; i < sprites.size(); ++i) {
		auto& sprite = sprites[i];

		//Spriteの更新
		sprite->Update();
	}

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

	for (auto& sprite : sprites) {
		//sprite描画処理
		sprite->Draw();
	}
}