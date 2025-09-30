#include "TitleScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

//初期化
void TitleScene::Initialize() {

	//サウンド
	Audio::GetInstance()->Initialize();

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.gltf");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("title.obj");
	ModelManager::GetInstance()->LoadModel("pushspace.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	//タイトルオブジェクト
	titleObject = std::make_unique<TitleObject>();
	titleObject->Initialize();

	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);

	//Skybox
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");
	//JsonManager
	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");

}

//終了
void TitleScene::Finalize() {
	//カメラマネージャ
	CameraManager::GetInstance()->Finalize();
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void TitleScene::Update() {
	//カメラ
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	//Skybox
	skybox->Update();
	//タイトルオブジェクト
	titleObject->Update();

	//シーン遷移
	SceneChange();

	//デバック
	Debug();
}

//描画
void TitleScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	//Skybox
	skybox->Draw();
	//タイトルオブジェクト
	titleObject->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}

//デバック
void TitleScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	Transform& trans = skybox->GetTransform();
	if (ImGui::TreeNode("SkyBox")) {
		ImGui::DragFloat3("Scale", &trans.scale.x, 0.01f, 0.01f, 1000.0f);
		ImGui::DragFloat3("Rotate", &trans.rotate.x, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Translate", &trans.translate.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::TreePop();
	}

	ImGui::End();
#endif
}

//シーン遷移
void TitleScene::SceneChange() {
	// フェード更新
	fade->Update();

	//FadeInが終わったらNone
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}

	//フェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}

	// フェードアウト終了後にシーン変更
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}
