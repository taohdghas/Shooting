#pragma once
#include "BaseScene.h"
#include "Fade.h"

//ゲームクリアシーン
class GameClearScene : public BaseScene
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
private:
	//フェード
	std::unique_ptr<Fade>fade;
};

