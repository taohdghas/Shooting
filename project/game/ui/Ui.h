#pragma once
#include "Sprite.h"
#include "Struct.h"
#include "Player.h"

class Ui
{
public:
	/// <summary>
	/// UIの初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// UIの更新
	/// </summary>
	void Update();
	/// <summary>
	/// UIの描画
	/// </summary>
	void Draw();
	/// <summary>
	/// UIのデバック
	/// </summary>
	void Debug();
	/// <summary>
	/// HPバー更新
	/// </summary>
	void UpdateHPBar();
     /// <summary>
     /// 操作説明画面処理
     /// </summary>
	void UpdateOperationGuide();
public:
	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 操作説明画面表示設定
	/// </summary>
	void SetOperationGuide(bool flag) { is_show_operation_ = flag; }
private:
	// HPバー
	std::unique_ptr<MyEngine::Sprite> hp_bar_;
	//操作説明
	std::unique_ptr<MyEngine::Sprite> operation_guide_;
	// プレイヤーへのポインタ
	Player* player_ = nullptr;
	//操作説明画面のスケール
	float operation_scale_ = 0.0f;
	//操作説明画面が出ているか
	bool is_show_operation_ = false;
};