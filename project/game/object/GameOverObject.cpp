#include "GameOverObject.h"
#include "ImGuiManager.h"

//初期化
void GameOverObject::Initialize() {
	//ゲームオーバーー字のオブジェクト
	gameover = std::make_unique<Object3d>();
	gameover->Initialize(Object3dBase::GetInstance());
	gameover->SetModel("gameover.obj");
	gameoverTransform.scale = { 1.0f,1.0f,1.0f };
	gameoverTransform.rotate = { 0.0f,0.0f,0.0f };
	gameoverTransform.translate = { -2.4f,0.5f,1.0f };
	
	//ゲームオーバー文字
	std::string models[NumLetters] = {
	   "g.obj","a.obj","m.obj","e.obj","o.obj","v.obj","e.obj","r.obj"
	};

	for (int i = 0; i < NumLetters; i++) {
		letters[i].obj = std::make_unique<Object3d>();
		letters[i].obj->Initialize(Object3dBase::GetInstance());
		letters[i].obj->SetModel(models[i]);

		letters[i].transform.scale = { 1.0f,1.0f,1.0f };
		letters[i].transform.rotate = { 0.0f,0.0f,0.0f };
		letters[i].transform.translate = { startX + spacing * i, baseY, 1.0f };

		letters[i].delay = i * 0.1f; 
		
	}

	//retryのオブジェクト
	retry = std::make_unique<Object3d>();
	retry->Initialize(Object3dBase::GetInstance());
	retry->SetModel("retry.obj");
	retryTransform.scale = { 0.7f,0.7f,0.7f };
	retryTransform.rotate = { 0.0f,0.0f,0.0f };
	retryTransform.translate = { -0.65f,-1.7f,1.0f };

}
//更新
void GameOverObject::Update() {

	//文字ジャンプ処理
	jumpTimer_ += DeltaTime;

	// 現在ジャンプ中の文字
	for (int i = 0; i < NumLetters; i++) {
		float y = 0.0f;

		if (i == currentIndex_) {
			//ジャンプ中
			float t = jumpTimer_ / jumpDuration_; 
			if (t < 1.0f) {
				y = sinf(t * 3.14159f) * jumpHeight_;
			} else {
				//次の文字へ移行
				jumpTimer_ = 0.0f;
				currentIndex_++;
				if (currentIndex_ >= NumLetters) {
					//最後まで行ったら最初に戻る
					currentIndex_ = 0; 
				}
			}
		}

		//反映
		letters[i].transform.translate.y = y;
		letters[i].obj->SetScale(letters[i].transform.scale);
		letters[i].obj->SetRotate(letters[i].transform.rotate);
		letters[i].obj->SetTranslate(letters[i].transform.translate);
		letters[i].obj->Update();
	}


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
	//ゲームオーバー文字のオブジェクト
	//gameover->Draw();
	for (int i = 0; i < NumLetters; i++) {
		letters[i].obj->Draw();
	}
	//retryのオブジェクト
	retry->Draw();
}
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