#include "TitleScene.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

// タイトルシーンの初期化処理
void TitleScene::Initialize() {

	// サウンド初期化
	MyEngine::Audio::GetInstance()->Initialize();

	// モデルの読み込み
	MyEngine::ModelManager::GetInstance()->LoadModel("titleobject/title.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("pushspace.obj");
	MyEngine::ModelManager::GetInstance()->LoadModel("player/player.obj");

	// カメラの初期化・登録
	camera_ = std::make_unique<MyEngine::Camera>();
	camera_->SetTranslate({ 0.0f,0.0f,-10.0f });
	MyEngine::CameraManager::GetInstance()->AddCamera("Main", camera_.get());
	MyEngine::CameraManager::GetInstance()->SetActiveCamera("Main");
	MyEngine::Object3dBase::GetInstance()->SetDefaultCamera(MyEngine::CameraManager::GetInstance()->GetActiveCamera());

	// タイトル画面用オブジェクトの初期化
	title_object_ = std::make_unique<TitleObject>();
	title_object_->Initialize();

	// フェードの初期化・開始
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	fade_->FadeStart(Fade::State::FadeIn, 0.5f);

	// Skyboxの初期化
	skybox_ = std::make_unique<Skybox>();
	skybox_->Initialize("resources/skybox/vz_classic_land_cubemap_ue.dds");

	// JsonManagerでレベルデータ読み込み
	json_manager_ = std::make_unique<MyEngine::JsonManager>();
	level_data_ = json_manager_->LoadJsonFile("untitled");
}

// タイトルシーンの終了処理
void TitleScene::Finalize() {
	// カメラマネージャの終了処理
	MyEngine::CameraManager::GetInstance()->Finalize();
	// サウンドの終了処理
	MyEngine::Audio::GetInstance()->Finalize();
}

// 毎フレームの更新処理
void TitleScene::Update() {
	// カメラの更新
	MyEngine::CameraManager::GetInstance()->GetActiveCamera()->Update();
	// Skyboxの更新
	skybox_->Update();
	// タイトルオブジェクトの更新
	title_object_->Update();

	//シーン遷移処理
	SceneChange();

	//デバッグ表示
	Debug();
}

// 描画処理
void TitleScene::Draw() {
	// 3Dオブジェクト描画準備
	MyEngine::Object3dBase::GetInstance()->DrawBaseSet();
	// Skyboxの描画
	skybox_->Draw();
	// タイトルオブジェクトの描画
	title_object_->Draw();

	// 共通描画設定
	MyEngine::SpriteBase::GetInstance()->DrawBaseSet();

	title_object_->DrawSprite();

	// フェード描画
	fade_->Draw();
}

// デバッグ表示（ImGuiによるパラメータ調整・状態表示）
void TitleScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SetUp");
	// カメラのパラメータ調整
	if (ImGui::TreeNode("Camera")) {
		Vector3 cameraPos = camera_->GetTranslate();
		Vector3 cameraRot = camera_->GetRotate();
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.01f);
		camera_->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera_->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	// Skyboxのパラメータ調整
	Transform& trans = skybox_->GetTransform();
	if (ImGui::TreeNode("SkyBox")) {
		ImGui::DragFloat3("Scale", &trans.scale.x, 0.01f, 0.01f, 1000.0f);
		ImGui::DragFloat3("Rotate", &trans.rotate.x, 0.1f, -360.0f, 360.0f);
		ImGui::DragFloat3("Translate", &trans.translate.x, 0.1f, -1000.0f, 1000.0f);
		ImGui::TreePop();
	}
	ImGui::End();
	// タイトルオブジェクトのデバッグ表示
	title_object_->Debug();
#endif
}

// シーン遷移処理
void TitleScene::SceneChange() {
	// フェードの更新
	fade_->Update();

	// フェードイン終了後にフェード状態をNoneへ
	if (fade_->GetState() == Fade::State::FadeIn && fade_->IsFinished()) {
		fade_->End();
	}

	auto result = title_object_->GetMenuResult();

	// 「始める」が選ばれた時だけフェードアウト
	if (fade_->GetState() == Fade::State::None &&
		result == TitleObject::MenuResult::Start) {

		fade_->FadeStart(Fade::State::FadeOut, 0.5f);
	}

	// フェードアウト終了後にゲームシーンへ遷移
	if (fade_->GetState() == Fade::State::FadeOut && fade_->IsFinished()) {
		MyEngine::SceneManager::GetInstance()->ChangeScene("GAME");
	}
	// 「終了」は即 exe を落とす（フェードなし）
	if (result == TitleObject::MenuResult::Exit) {
		PostQuitMessage(0);
	}
}
