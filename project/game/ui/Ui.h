#pragma once
#include "Sprite.h"
#include "Struct.h"
#include "Player.h"
#include <unordered_map>
#include <memory>

// UI管理クラス
class Ui
{
public:
	// スプライトの種類
	enum class SpriteType {
		HpBar,//HPバー
		Pause,//ポーズ表示
		Retry,//リトライ文字
		BackTitle,//タイトルへ戻る文字
		OperationGuide,//操作説明画面
		KeyA,//Aキー
		KeyD,//Dキー
		KeyF,//Fキー
		KeyW,//Wキー
		MouseLeft,//マウス左
		MouseMove,//マウス移動
	};
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
     /// ポーズ画面処理
     /// </summary>
	void UpdatePauseGuide();
	/// <summary>
	/// スプライト作成
	/// </summary>
	void CreateSprite(SpriteType type,const char* path,const Vector2& pos,
		const Vector2& size,const Vector2& anchor = { 0,0 });

public:
	/// <summary>
	/// スプライト取得
	/// </summary>
	MyEngine::Sprite* Get(SpriteType type);

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 操作説明画面表示設定
	/// </summary>
	void SetShowPause(bool flag) { is_show_pause_ = flag; }
private:
	// スプライト格納用マップ
	std::unordered_map<SpriteType, std::unique_ptr<MyEngine::Sprite>> sprites_;
	// プレイヤーへのポインタ
	Player* player_ = nullptr;
	//リトライ文字の基準サイズ
	Vector2 retry_base_size = { 100.0f,100.0f };
	//タイトルㇸ戻る文字の基準サイズ
	Vector2 back_title_base_size = { 100.0f,100.0f };
	//操作説明画面の基準サイズ
	Vector2 operation_base_size = { 500.0f,500.0f };
	//リトライ文字のスケール
	float retry_scale_ = 0.0f;
	//タイトルへ戻る文字のスケール
	float back_title_scale_ = 0.0f;
	//ポーズ画面のスプライトスケール
	float pause_scale_ = 0.0f;
	//ポーズ画面が出ているか
	bool is_show_pause_ = false;
};