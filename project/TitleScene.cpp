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
	TextureManager::GetInstance()->LoadTexture("resources/pushspacecolor.png");

	//モデル読み込み
	ModelManager::GetInstance()->LoadModel("plane.gltf");
	ModelManager::GetInstance()->LoadModel("axis.obj");
	ModelManager::GetInstance()->LoadModel("pushspace.obj");

	//カメラ
	camera = std::make_unique<Camera>();
	camera->SetRotate({ 0.0f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,0.0f,-10.0f });

	pushspace = std::make_unique<Object3d>();
	pushspace->Initialize(Object3dBase::GetInstance());
	pushspace->SetModel("pushspace.obj");
	pushspace->SetTranslate({ 0.0f,0.5f,0.0f });
}

//終了
void TitleScene::Finalize() {
	//Audio
	Audio::GetInstance()->Finalize();
}

//更新
void TitleScene::Update() {
	camera->Update();

	pushspace->Update();

	//エンターキーを押したらゲームシーンへ
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		pushspaceMove = true;
	}

	if (pushspaceMove) {
		Vector3 pushspaceTranslate = pushspace->GetTranslate();
		pushspaceTranslate.z -= 1.0f;
		pushspace->SetTranslate(pushspaceTranslate);
		if (pushspaceTranslate.z <= -300.0f) {
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
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
	ImGui::End();
#endif
}

//描画
void TitleScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	pushspace->Draw();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

}