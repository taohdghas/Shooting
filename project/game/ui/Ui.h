#pragma once
#include "Sprite.h"
#include "Struct.h"
#include "Player.h"
#include <unordered_map>
#include <memory>

// UIクラス
class Ui
{
public:
	// スプライトの種類
	enum class SpriteType {
		HpBar,
		Pause,
		OperationGuide,
		KeyA,
		KeyD,
		KeyF,
		KeyW,
		MouseLeft,
		MouseMove,
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
     /// 操作説明画面処理
     /// </summary>
	void UpdateOperationGuide();
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
	void SetOperationGuide(bool flag) { is_show_operation_ = flag; }
private:
	// スプライト格納用マップ
	std::unordered_map<SpriteType, std::unique_ptr<MyEngine::Sprite>> sprites_;
	// プレイヤーへのポインタ
	Player* player_ = nullptr;
	//操作説明画面のスケール
	float operation_scale_ = 0.0f;
	//操作説明画面が出ているか
	bool is_show_operation_ = false;
};