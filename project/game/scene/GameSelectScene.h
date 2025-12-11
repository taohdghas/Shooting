#pragma once
#include "BaseScene.h"

//ゲームセレクトシーン
class GameSelectScene : public  MyEngine::BaseScene
{
public:
	/// <summary>
	/// シーンの初期化処理を行う。
	/// </summary>
	void Initialize()override;
	/// <summary>
	/// シーンの終了処理を行う。
	/// </summary>
	void Finalize()override;
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画処理を行う。
	/// </summary>
	void Draw()override;
	/// <summary>
	/// デバッグ用の処理を行う。
	/// </summary>
	void Debug()override;
private:

};

