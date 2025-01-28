#include "MyGame.h"
#include "SceneFactory.h"

//初期化
void MyGame::Initialize() {

	//基底クラスの初期化
	Framework::Initialize();

	//シーンファクトリーを生成してマネージャにセット
	sceneFactory_ = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory_);
    //シーンマネージャに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

//終了
void MyGame::Finalize() {
	//基底クラスの終了
	Framework::Finalize();
}

//毎フレーム更新
void MyGame::Update() {
	
	//基底クラスの更新
	Framework::Update();

	//ImGui開始
	imguimanager->Begin();

	//ImGui終了
	imguimanager->End();
}

//描画
void MyGame::Draw() {
	//描画前処理
	directxBase->PreDraw();

	srvManager->PreDraw();

	//シーンマネージャ描画
	SceneManager::GetInstance()->Draw();

	//ImGui描画
	imguimanager->Draw();

	//描画後処理
	directxBase->PostDraw();
}
