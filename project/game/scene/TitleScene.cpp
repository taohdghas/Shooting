#include "TitleScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

// タイトルシーンの初期化処理
void TitleScene::Initialize() {

	// サウンド初期化
	Audio::GetInstance()->Initialize();

	// モデルの読み込み
	ModelManager::GetInstance()->LoadModel("plane.gltf");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("title.obj");
	ModelManager::GetInstance()->LoadModel("pushspace.obj");
	ModelManager::GetInstance()->LoadModel("player/player.obj");

	// カメラの初期化・登録
	camera = std::make_unique<Camera>();
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	CameraManager::GetInstance()->AddCamera("Main", camera.get());
	CameraManager::GetInstance()->SetActiveCamera("Main");
	Object3dBase::GetInstance()->SetDefaultCamera(CameraManager::GetInstance()->GetActiveCamera());

	// タイトル画面用オブジェクトの初期化
	titleObject = std::make_unique<TitleObject>();
	titleObject->Initialize();

	// フェードの初期化・開始
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);

	// Skyboxの初期化
	skybox = std::make_unique<Skybox>();
	skybox->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// JsonManagerでレベルデータ読み込み
	jsonManager = std::make_unique<JsonManager>();
	levelData = jsonManager->LoadJsonFile("untitled");
}

// タイトルシーンの終了処理
void TitleScene::Finalize() {
	// カメラマネージャの終了処理
	CameraManager::GetInstance()->Finalize();
	// サウンドの終了処理
	Audio::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void TitleScene::Update() {
	// カメラの更新
	CameraManager::GetInstance()->GetActiveCamera()->Update();
	// Skyboxの更新
	skybox->Update();
	// タイトルオブジェクトの更新
	titleObject->Update();

	// シーン遷移処理
	SceneChange();

	// デバッグ表示
	Debug();
}

// 描画処理
void TitleScene::Draw() {
	// 3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();
	// Skyboxの描画
	skybox->Draw();
	// タイトルオブジェクトの描画
	titleObject->Draw();

	// 共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	// フェード描画
	fade->Draw();
}

// デバッグ表示（ImGuiによるパラメータ調整・状態表示）
void TitleScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	// カメラのパラメータ調整
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera->GetTranslate();
		Vector3 cameraRot = camera->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	// Skyboxのパラメータ調整
	Transform& trans = skybox->GetTransform();
	if (ImGui::TreeNode("SkyBox")) {
		ImGui::DragFloat3("Scale", &trans.scale.x, 0.01f, 0.01f, 1000.0f);
		ImGui::DragFloat3("Rotate", &trans.rotate.x, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Translate", &trans.translate.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::TreePop();
	}
	ImGui::End();
	// タイトルオブジェクトのデバッグ表示
	titleObject->Debug();
#endif
}

// シーン遷移処理
void TitleScene::SceneChange() {
	// フェードの更新
	fade->Update();

	// フェードイン終了後にフェード状態をNoneへ
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}

	// スペースキー押下でフェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}

	// フェードアウト終了後にゲームシーンへ遷移
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}
