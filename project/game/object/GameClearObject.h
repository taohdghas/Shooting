#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

//ゲームクリアオブジェクト
class GameClearObject
{
public:
	/// <summary>
	/// ゲームクリアオブジェクトの初期化処理を行う。
	/// 必要なリソースやオブジェクトの生成・初期化を行う。
	/// </summary>
	void Initialize();

	/// <summary>
	/// ゲームクリアオブジェクトの毎フレーム更新処理を行う。
	/// 文字のジャンプアニメーションやα値の更新などを管理する。
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームクリアオブジェクトの描画処理を行う。
	/// 各文字やオブジェクトの描画コマンドを発行する。
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲームクリアオブジェクトのデバッグ表示・デバッグ用更新処理を行う。
	/// パラメータ調整やデバッグ情報の表示を行う。
	/// </summary>
	void Debug();

private:
	//ゲームクリア文字
	std::unique_ptr<Object3d>gameClear_;
	//ゲームクリア文字トランスフォーム
	Transform gameClearTransform_;

};
