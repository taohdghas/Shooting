#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"

//初期化
void GameClearScene::Initialize() {

}

//終了
void GameClearScene::Finalize() {

}

//更新
void GameClearScene::Update() {

	//タイトルへ
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

//描画
void GameClearScene::Draw() {

}

//デバック
void GameClearScene::Debug() {

}