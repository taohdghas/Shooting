#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Object3dBase.h"
#include "SpriteBase.h"

//初期化
void GameClearScene::Initialize() {
	//フェード
	fade = std::make_unique<Fade>();
	fade->Initialize();
	fade->FadeStart(Fade::State::FadeIn, 0.5f);
}

//終了
void GameClearScene::Finalize() {

}

//更新
void GameClearScene::Update() {

	//フェード
	fade->Update();

	//タイトルへ
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

//描画
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