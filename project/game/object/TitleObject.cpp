#include "TitleObject.h"
#include "ImGuiManager.h"

// タイトル画面用オブジェクトの初期化処理
void TitleObject::Initialize() {
	// タイトルのオブジェクト生成・初期化
	title = std::make_unique<Object3d>();
	title->Initialize(Object3dBase::GetInstance());
	title->SetModel("title.obj");
	title->SetScale({ 1.2f,1.2f,1.2f });
	title->SetTranslate({ -1.2f,0.8f,1.0f });

	// pushspaceのオブジェクト生成・初期化
	pushspace = std::make_unique<Object3d>();
	pushspace->Initialize(Object3dBase::GetInstance());
	pushspace->SetModel("pushspace.obj");
	pushspace->SetScale({ 0.5f,0.5f,0.5f });
	pushspace->SetTranslate({ 1.1f,-1.5f,1.0f });

	// プレイヤーオブジェクト（外見のみ）生成・初期化
	playerobj = std::make_unique<Object3d>();
	playerobj->Initialize(Object3dBase::GetInstance());
	playerobj->SetModel("player/player.obj");
	playerobjTransform.scale = { 0.5f,0.5f,0.5f };
	playerobjTransform.rotate = { 0.0f,-0.5f,0.0f };
	playerobjTransform.translate = { -1.3f,-0.5f,-4.8f };
}

// 毎フレームの更新処理
void TitleObject::Update() {
	// タイトルオブジェクトの更新
	title->Update();

	// pushspaceの点滅演出（アルファ値を周期的に変化させる）
	alphaTimer_ += DeltaTime;
	alpha_ = (sinf(alphaTimer_ * 3.0f) * 0.5f) + 0.5f;
	pushspace->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
	pushspace->Update();

	// プレイヤーオブジェクトの演出（回転・ジャンプ処理）
	PlayerObjDirection();
}

// 描画処理
void TitleObject::Draw() {
	title->Draw();
	pushspace->Draw();
	playerobj->Draw();
}

// プレイヤーオブジェクトの演出（回転・ジャンプ処理）
void TitleObject::PlayerObjDirection() {
	// プレイヤーオブジェクトの回転
	playerobjTransform.rotate.y += RotateSpeed * DeltaTime;

	// ジャンプタイマー更新
	jumpTimer_ += 1.0f;
	// ジャンプ開始判定
	if (!isJumping_ && jumpTimer_ >= jumpInterval_) {
		jumpVelocity_ = jumpPower_;
		isJumping_ = true;
		jumpCount_ = 1;
		jumpTimer_ = 0.0f;
	}

	// ジャンプ中の処理
	if (isJumping_) {
		playerobjTransform.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		// 2段目ジャンプ判定
		if (jumpCount_ == 1 && jumpVelocity_ < 0.0f) {
			jumpVelocity_ = jumpPower_ * 1.2f; 
			jumpCount_ = 2; 
		}

		// 2段目ジャンプ時の回転
		if (jumpCount_ == 2) {
			playerobjTransform.rotate.x += jumpRotateSpeed_ * DeltaTime;
		}

		// 着地判定
		if (playerobjTransform.translate.y <= groundY_) {
			playerobjTransform.translate.y = groundY_;
			jumpVelocity_ = 0.0f;
			isJumping_ = false;
			jumpCount_ = 0;
			playerobjTransform.rotate.x = 0.0f; 
		}
	}

	// Transform情報をObject3dへ反映
	playerobj->SetScale(playerobjTransform.scale);
	playerobj->SetRotate(playerobjTransform.rotate);
	playerobj->SetTranslate(playerobjTransform.translate);
	playerobj->Update();
}

// デバッグ表示（ImGuiによるパラメータ調整）
void TitleObject::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("TitleObject SetUp");

	// タイトルオブジェクトのパラメータ調整
	static float titleScale[3] = { 1.2f,1.2f,1.2f };
	static float titleRotate[3] = { 0.0f, 0.0f, 0.0f };
	static float titleTranslate[3] = { -1.2f, 0.8f, 1.0f };

	ImGui::Text("Title");
	if (ImGui::DragFloat3("Title Scale", titleScale, 0.01f)) {
		title->SetScale({ titleScale[0], titleScale[1], titleScale[2] });
	}
	if (ImGui::DragFloat3("Title Rotate", titleRotate, 0.5f)) {
		title->SetRotate({ titleRotate[0], titleRotate[1], titleRotate[2] });
	}
	if (ImGui::DragFloat3("Title Translate", titleTranslate, 0.01f)) {
		title->SetTranslate({ titleTranslate[0], titleTranslate[1], titleTranslate[2] });
	}

	ImGui::Separator();

	// pushspaceオブジェクトのパラメータ調整
	static float pushScale[3] = { 0.5f, 0.5f, 0.5f };
	static float pushRotate[3] = { 0.0f, 0.0f, 0.0f };
	static float pushTranslate[3] = { 1.1f, -1.5f, 1.0f };

	ImGui::Text("PushSpace");
	if (ImGui::DragFloat3("Push Scale", pushScale, 0.01f)) {
		pushspace->SetScale({ pushScale[0], pushScale[1], pushScale[2] });
	}
	if (ImGui::DragFloat3("Push Rotate", pushRotate, 0.5f)) {
		pushspace->SetRotate({ pushRotate[0], pushRotate[1], pushRotate[2] });
	}
	if (ImGui::DragFloat3("Push Translate", pushTranslate, 0.01f)) {
		pushspace->SetTranslate({ pushTranslate[0], pushTranslate[1], pushTranslate[2] });
	}

	ImGui::Separator();

	// プレイヤーオブジェクトのパラメータ調整
	static float playerScale[3] = { 0.5f, 0.5f, 0.5f };
	static float playerRotate[3] = { 0.0f, 0.0f, 0.0f };
	static float playerTranslate[3] = { -1.3f,0.0f,-4.8f };

	ImGui::Text("PlayerObj");
	if (ImGui::DragFloat3("Player Scale", playerScale, 0.01f)) {
		playerobjTransform.scale = { playerScale[0], playerScale[1], playerScale[2] };
	}
	if (ImGui::DragFloat3("Player Rotate", playerRotate, 0.5f)) {
		playerobjTransform.rotate = { playerRotate[0], playerRotate[1], playerRotate[2] };
	}
	if (ImGui::DragFloat3("Player Translate", playerTranslate, 0.01f)) {
		playerobjTransform.translate = { playerTranslate[0], playerTranslate[1], playerTranslate[2] };
	}

	ImGui::End();
#endif
}