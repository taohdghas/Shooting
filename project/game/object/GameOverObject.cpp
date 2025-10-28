#include "GameOverObject.h"
#include "ImGuiManager.h"

//初期化
void GameOverObject::Initialize() {
	//ゲームオーアー字のオブジェクト
	gameover = std::make_unique<Object3d>();
	gameover->Initialize(Object3dBase::GetInstance());
	gameover->SetModel("gameover.obj");
	gameoverTransform.scale = { 1.0f,1.0f,1.0f };
	gameoverTransform.rotate = { 0.0f,0.0f,0.0f };
	gameoverTransform.translate = { -2.4f,0.5f,1.0f };

	//retryのオブジェクト
	retry = std::make_unique<Object3d>();
	retry->Initialize(Object3dBase::GetInstance());
	retry->SetModel("retry.obj");
	retryTransform.scale = { 0.7f,0.7f,0.7f };
	retryTransform.rotate = { 0.0f,0.0f,0.0f };
	retryTransform.translate = { -0.5f,-1.7f,1.0f };
	
}
//更新
void GameOverObject::Update() {

	//ゲームオーバートランスフォームのセット
	gameover->SetScale(gameoverTransform.scale);
	gameover->SetRotate(gameoverTransform.rotate);
	gameover->SetTranslate(gameoverTransform.translate);

	//retryの点滅
	alphaTimer_ += DeltaTime;
	alpha_ = (sinf(alphaTimer_ * 3.0f) * 0.5f) + 0.5f;
	retry->SetColor({ 1.0f,1.0f,1.0f,alpha_ });

	//retryトランスフォームのセット
	retry->SetScale(retryTransform.scale);
	retry->SetRotate(retryTransform.rotate);
	retry->SetTranslate(retryTransform.translate);

	//ゲームオーアー字のオブジェクト
	gameover->Update();
	//retryのオブジェクト
	retry->Update();
}
//描画
void GameOverObject::Draw() {
	//ゲームオーアー字のオブジェクト
	gameover->Draw();
	//retryのオブジェクト
	retry->Draw();
}
//デバック
//デバック
void GameOverObject::Debug() {
#ifdef USE_IMGUI
	if (ImGui::Begin("GameOverObject SRT")) {
		// GameOver SRT
		ImGui::Text("GameOver");
		ImGui::DragFloat3("GameOver Scale", &gameoverTransform.scale.x, 0.01f);
		ImGui::DragFloat3("GameOver Rotate", &gameoverTransform.rotate.x, 0.01f);
		ImGui::DragFloat3("GameOver Translate", &gameoverTransform.translate.x, 0.01f);

		ImGui::Separator();

		// Retry SRT
		ImGui::Text("Retry");
		ImGui::DragFloat3("Retry Scale", &retryTransform.scale.x, 0.01f);
		ImGui::DragFloat3("Retry Rotate", &retryTransform.rotate.x, 0.01f);
		ImGui::DragFloat3("Retry Translate", &retryTransform.translate.x, 0.01f);
	}
	ImGui::End();
#endif
}