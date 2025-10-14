#include "GameOverScene.h"
#include "Object3dBase.h"
#include "SpriteBase.h"
#include "Input.h"
#include "SceneManager.h"

//初期化
void GameOverScene::Initialize() {
	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);
}
//終了
void GameOverScene::Finalize() {

}
//更新
void GameOverScene::Update() {
	//フェード
	fade->Update();
	//シーン遷移
	SceneChange();
}
//描画
void GameOverScene::Draw() {
	//3Dオブジェクト描画準備
	Object3dBase::GetInstance()->DrawBaseSet();

	//共通描画設定
	SpriteBase::GetInstance()->DrawBaseSet();

	//フェード
	fade->Draw();
}
//デバック
void GameOverScene::Debug() {

}
//シーン遷移
void GameOverScene::SceneChange() {
	//フェードインが終わったら状態リセット
	if (fade->GetState() == Fade::State::FadeIn && fade->IsFinished()) {
		fade->End();
	}
	//タイトルへのフェードアウト開始
	if (fade->GetState() == Fade::State::None && Input::GetInstance()->PushKey(DIK_SPACE)) {
		fade->FadeStart(Fade::State::FadeOut, 0.5f);
	}
	//フェードアウト後シーン移行
	if (fade->GetState() == Fade::State::FadeOut && fade->IsFinished()) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}