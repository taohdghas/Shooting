#pragma once
#include "Sprite.h"
#include "Struct.h"
#include "Player.h"
#include <unordered_map>
#include <memory>

// UIクラス
/// <summary>
/// ゲーム画面上の各種UIの表示・更新・描画管理
/// スプライトの生成・管理・状態制御
/// プレイヤー情報とUI表示の連携
/// ポーズメニューの選択・クリック・ホバー判定や結果管理
/// 操作説明画面や各種ガイドの表示制御
/// UIに関するデバッグ表示や状態リセット等の補助機能の提供
/// </summary>
class Ui
{
public:
	//ポーズメニューの結果
	enum class PauseResult {
		None,//何もしない
		Retry,//リトライ
		BackTitle//タイトルへ戻る
	};
	//スプライトの種類
	enum class SpriteType {
		HpBar,//HPバー
		HpBarFlash,//HPバーフラッシュ
		HpBarCaution,//HPバー注意
		HpBarCautionFlash,//HPバー注意フラッシュ
		HpBarDanger,//HPバー危険
		hpBarDangerFlash,//HPバー危険フラッシュ
		DodgeGauge,//回避ゲージ
		DodgeGaugeFlash,//回避ゲージフラッシュ
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
	/// ポーズボタンホバー処理
	/// </summary>
	void UpdatePauseButtonHover();
	/// <summary>
	/// ポーズメニュークリック処理
	/// </summary>
	void UpdatePauseClick();
	/// <summary>
    /// 回避クールタイムバー更新
    /// </summary>
	void UpdateDodgeGauge();
	/// <summary>
	/// スプライト作成
	/// </summary>
	void CreateSprite(SpriteType type, const char* path, const Vector2& pos,
		const Vector2& size, const Vector2& anchor = { 0,0 });
	/// <summary>
	/// スプライト上にマウスがあるか
	/// </summary>
	bool IsMouseOnSprite(MyEngine::Sprite* sprite);

public:
	/// <summary>
	/// メニュー選択結果リセット
	/// </summary>
	void ResetPauseResult() { pause_result_ = PauseResult::None; }

	/// <summary>
    /// メニュー選択結果を取得
    /// </summary>
	PauseResult GetPauseResult() const { return pause_result_; }

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
	//ポーズメニュー選択結果
	PauseResult pause_result_ = PauseResult::None;
	// 現在使用中のHPバー
	SpriteType current_hp_bar_ = SpriteType::HpBar;
	SpriteType current_hp_flash_ = SpriteType::HpBarFlash;
	// スプライト格納用マップ
	std::unordered_map<SpriteType, std::unique_ptr<MyEngine::Sprite>> sprites_;
	// プレイヤーへのポインタ
	Player* player_ = nullptr;
	//リトライ文字の基準サイズ
	Vector2 retry_base_size_ = { 180.0f,100.0f };
	//タイトルㇸ戻る文字の基準サイズ
	Vector2 back_title_base_size_ = { 180.0f,100.0f };
	//操作説明画面の基準サイズ
	Vector2 operation_base_size_ = { 500.0f,500.0f };
	//ポーズ画面のスプライトスケール
	float pause_scale_ = 0.0f;
	//ホバー用タイマー
	float hover_timer_ = 0.0f;
	//回避ゲージフラッシュ用タイマー
	float dodge_flash_timer_ = 0.0f;
	//HPバーフラッシュ用タイマー
	float hp_flash_timer_ = 0.0f;
	//前フレームのHP比率
	float prev_hp_ratio_ = 1.0f;
	//表示用HP比率
	float display_hp_ratio_ = 1.0f;
	//回避ゲージフラッシュ表示フラグ
	bool is_dodge_flash_visible_ = true;
	//ポーズ画面が出ているか
	bool is_show_pause_ = false;
	//HPバーフラッシュ表示フラグ
	bool is_hp_flash_visible_ = false;
};