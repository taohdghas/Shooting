#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Object3dBase.h"
#include "SpriteBase.h"

// ゲームクリアシーンの初期化処理
void GameClearScene::Initialize() {
	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);
}

// ゲームクリアシーンの終了処理
void GameClearScene::Finalize() {

}

// 毎フレームの更新処理
void GameClearScene::Update() {

	//フェード
	fade->Update();
	//シーン遷移
	SceneChange();

}

// ゲームクリアシーンの描画処理
void GameClearScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}
//デバック
void GameClearScene::Debug() {

}
//シーン遷移
void GameClearScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//タイトルへのフェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->IsKeyPressed(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}