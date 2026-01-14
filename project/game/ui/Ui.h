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
	//ポーズ
	std::unique_ptr<MyEngine::Sprite> pause_;
	//操作説明
	std::unique_ptr<MyEngine::Sprite> operation_guide_;
	//AKey
	std::unique_ptr<MyEngine::Sprite> a_key_sprite_;
	//DKey
	std::unique_ptr<MyEngine::Sprite> d_key_sprite_;
	//FKey
	std::unique_ptr<MyEngine::Sprite> f_key_sprite_;
	//WKey
	std::unique_ptr<MyEngine::Sprite> w_key_sprite_;
	//SpaceKey
	std::unique_ptr<MyEngine::Sprite> space_key_sprite_;
	//マウス左スプライト
	std::unique_ptr<MyEngine::Sprite> mouse_left_sprite_;
	//マウス移動スプライト
	std::unique_ptr<MyEngine::Sprite> mouse_move_sprite_;

	// プレイヤーへのポインタ
	Player* player_ = nullptr;
	//操作説明画面のスケール
	float operation_scale_ = 0.0f;
	//操作説明画面が出ているか
	bool is_show_operation_ = false;
};