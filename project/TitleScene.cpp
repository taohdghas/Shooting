#include "TitleScene.h"
#include "SceneManager.h"
#include "ImGuiManager.h"

//初期化
void TitleScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//テクスチャ読み込み
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.gltf");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	ModelData model = Model::LoadModelFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	Animation animation = Model::LoadAnimationFile("./resources/AnimatedCube", "AnimatedCube.gltf");
	//Sprite初期化
	for (uint32_t i = 0; i < 1; ++i) {
		auto sprite = std::make_unique<Sprite>();
		sprite->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(std::move(sprite));
	}
	sprites[0]->Initialize(SpriteBase::GetInstance(), "resources/uvChecker.png");

	//オブジェクト
	object3d = std::make_unique<Object3d>();
	object3d->Initialize(Object3dBase::GetInstance());
	object3d->SetTranslate({ 0.0f,0.0f,0.0f });
	object3d->SetModel("axis.obj");


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
	object3d->SetCamera(camera.get());
}

//終了
void TitleScene::Finalize() {
	ParticleManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void TitleScene::Update() {
	camera->Update();

	object3d->Update();

	//エンターキーを押したらゲームシーンへ
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.1f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.1f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Object")) {
		Vector3 objectScale = object3d->GetScale();
		Vector3 objectRotate = object3d->GetRotate();
		Vector3 objectTranslate = object3d->GetTranslate();
		ImGui::DragFloat3("ObjectScale", &objectScale.x, 0.1f);
		ImGui::DragFloat3("ObjectRotate", &objectRotate.x, 0.1f);
		ImGui::DragFloat3("ObjectTranslate", &objectTranslate.x, 0.1f);
		object3d->SetScale({ objectScale.x,objectScale.y,objectScale.z });
		object3d->SetRotate({ objectRotate.x,objectRotate.y,objectRotate.z });
		object3d->SetTranslate({ objectTranslate.x,objectTranslate.y,objectTranslate.z });
		ImGui::TreePop();
	}
	object3d->DebugUpdate();
	ImGui::End();
#endif
}

//描画
void TitleScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	object3d->Draw();

}