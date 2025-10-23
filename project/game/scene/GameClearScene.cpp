#include "GameClearScene.h"
#include "SceneManager.h"
#include "Input.h"

// ゲームクリアシーンの初期化処理
void GameClearScene::Initialize() {

}

// ゲームクリアシーンの終了処理
void GameClearScene::Finalize() {

}

// 毎フレームの更新処理
void GameClearScene::Update() {
	// Enterキーでタイトルシーンへ遷移
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

// ゲームクリアシーンの描画処理
void GameClearScene::Draw() {

}

// デバッグ表示（必要に応じて追加）
void GameClearScene::Debug() {

}