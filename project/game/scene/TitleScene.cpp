#include "TitleScene.h"
#include "SceneManager.h"
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
	playerobjTransform.scale = { 0.8f,0.8f,0.8f };
	playerobjTransform.rotate = { 0.0f,0.0f,0.0f };
	playerobjTransform.translate = { 0.0f,0.0f,0.0f };

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });
	pushspace->SetCamera(camera.get());

	//フェードマネージャー
	fadeManager = std::make_unique<FadeManager>();
	fadeManager->Initialize();
	fadeManager->FadeIn(1.0f);
}

//終了
void TitleScene::Finalize() {
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

	//演出
	Effect();

	//プレイヤーオブジェクト
	playerobj->Update();

	//シーン遷移
	SceneChange();

	fadeManager->Update();

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
	//playerobj->Draw();

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
		ImGui::DragFloat3("CameraTranslate", &cameraPos.x, 0.1f);
		ImGui::DragFloat3("CameraRotate", &cameraRot.x, 0.1f);
		camera->SetTranslate({ cameraPos.x,cameraPos.y,cameraPos.z });
		camera->SetRotate({ cameraRot.x,cameraRot.y,cameraRot.z });
		ImGui::TreePop();
	}
	ImGui::End();
#endif
}

//演出
void TitleScene::Effect() {
	//プレイヤーオブジェクトの回転
	playerobjTransform.rotate.x += RotateSpeed * kDeltaTime;

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