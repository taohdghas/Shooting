#pragma once
#include "BaseScene.h"
#include "Fade.h"
#include "GameOverObject.h"
#include "Camera.h"

//ゲームオーバーシーン
class GameOverScene : public BaseScene
{
public:
	//初期化
	void Initialize()override;
	//終了
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
	//デバック
	void Debug()override;
public:
	//シーン遷移
	void SceneChange();
private:
	//カメラ
	std::unique_ptr<Camera>camera;
	//フェード
	std::unique_ptr<Fade>fade;
	//ゲームオーバーオブジェクト
	std::unique_ptr<GameOverObject>gameOverObject;
	//タイトルシーンへの移行フラグ
	bool isToTitle = false;
	//ゲームシーンへの移行フラグ
	bool isToGame = false;
};