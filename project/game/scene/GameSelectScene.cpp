#include "GameSelectScene.h"
#include "SceneManager.h"
#include "Input.h"

// ゲームセレクトシーンの初期化処理
void GameSelectScene::Initialize() {

}

// ゲームセレクトシーンの終了処理
void GameSelectScene::Finalize() {

}

// 毎フレームの更新処理
void GameSelectScene::Update() {
	// Enterキーでゲームシーンへ遷移
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

// ゲームセレクトシーンの描画処理
void GameSelectScene::Draw() {

}

// デバッグ表示（必要に応じて追加）
void GameSelectScene::Debug() {

}