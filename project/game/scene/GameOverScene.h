#pragma once
#include "BaseScene.h"
#include "Fade.h"

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
	//フェード
	std::unique_ptr<Fade>fade;
	//タイトルシーンへの移行フラグ
	bool isToTitle = false;
	//ゲームシーンへの移行フラグ
	bool isToGame = false;
};