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

	//タイトルのオブジェクト
	title = std::make_unique<Object3d>();
	title->Initialize(Object3dBase::GetInstance());
	title->SetModel("title.obj");
	title->SetTranslate({ 0.0f,1.2f,1.0f });

	//pushspaceのオブジェクト
	pushspace = std::make_unique<Object3d>();
	pushspace->Initialize(Object3dBase::GetInstance());
	pushspace->SetModel("pushspace.obj");
	pushspace->SetTranslate({ 0.0f,-1.0f,1.0f });
	pushspace->SetScale({ 0.5f,0.5f,0.5f });

	//プレイヤーオブジェクト(外見のみ)
	playerobj = std::make_unique<Object3d>();
	playerobj->Initialize(Object3dBase::GetInstance());
	playerobj->SetModel("player/player.obj");
	playerobjTransform.scale = { 0.5f,0.5f,0.5f };
	playerobjTransform.rotate = { 0.0f,0.0f,0.0f };
	playerobjTransform.translate = { 0.0f,0.0f,0.0f };

	//フェードマネージャー
	fadeManager = std::make_unique<FadeManager>();
	fadeManager->Initialize();
	fadeManager->FadeIn(1.0f);
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
	camera->Update();

	//タイトルオブジェクト
	title->Update();
	//pushspaceオブジェクト
	pushspace->Update();

	//プレイヤーObj演出
	PlayerObjEffect();

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

	//シーン遷移
	//SceneChange();

	//fadeManager->Update();

	//デバック
	Debug();
}

//描画
void TitleScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//タイトルオブジェクト
	title->Draw();
	//pushspaceオブジェクト
	pushspace->Draw();
	//プレイヤーオブジェクト
	playerobj->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェードマネージャ
	fadeManager->Draw();
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
	ImGui::End();
#endif
}

//プレイヤーObj演出
void TitleScene::PlayerObjEffect() {
	//プレイヤーオブジェクトの回転
	playerobjTransform.rotate.y += RotateSpeed * kDeltaTime;
	playerobj->SetScale(playerobjTransform.scale);
	playerobj->SetRotate(playerobjTransform.rotate);
	playerobj->SetTranslate(playerobjTransform.translate);
	playerobj->Update();
}

//シーン遷移
void TitleScene::SceneChange() {
	//エンターキーを押したらゲームシーンへ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		pushspaceMove = true;
	}

	if (pushspaceMove) {
		Vector3 pushspaceTranslate = pushspace->GetTranslate();
		Vector3 titleTranslate = title->GetTranslate();
		pushspaceTranslate.z -= 3.0f;
		titleTranslate.z -= 3.0f;
		pushspace->SetTranslate(pushspaceTranslate);
		title->SetTranslate(titleTranslate);

		if (!fadeManager->IsFade() && pushspaceTranslate.z <= -300.0f) {
			fadeManager->FadeOut(1.0f);
		}
	}

	if (fadeManager->IsFadeOutEnd()) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}