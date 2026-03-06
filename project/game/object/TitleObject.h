#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include <vector>

// タイトルオブジェクト
/// <summary>
/// タイトル画面の表示
/// メニュー選択肢の管理と選択状態・決定判定
/// スプライトや3Dオブジェクトの初期化・更新・描画処理
/// プレイヤーオブジェクトの演出制御
/// メニュー項目の拡縮アニメーションやマウス操作による選択判定
/// 操作説明表示の制御と更新
/// デバッグUIの表示や状態リセット等の補助機能の提供
/// </summary>
class TitleObject {
public:
	/// メニュー選択結果
	enum class MenuResult {
		None,
		Start,
		Exit
	};

	/// <summary>
	/// 初期化
	/// <returns>なし</returns>
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <returns>なし</returns>
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// スプライト描画
	/// <returns>なし</returns>
	/// </summary>
	void DrawSprite();

	/// <summary>
	/// プレイヤーオブジェクトの演出処理
	/// <returns>なし</returns>
	/// </summary>
	void PlayerObjDirection();

	/// <summary>
	/// メニュー項目の拡縮処理
	/// <returns>なし</returns>
	/// </summary>
	void MenuSizeUpdate();

	/// <summary>
	/// 操作説明表示の更新
	/// <returns>なし</returns>
	/// </summary>
	void UpdateHowto();

	/// <summary>
	/// メニュー選択結果リセット
	/// <returns>なし</returns>
	/// </summary>
	void ResetMenuResult() { menu_result_ = MenuResult::None; }

	/// <summary>
	/// デバッグ用UIを表示
	/// <returns>なし</returns>
	/// </summary>
	void Debug();
	
	/// <summary>
	/// マウスが乗っているメニュー項目のインデックスを取得
	/// <returns>マウスが乗っているメニューインデックス</returns>
	/// </summary>
	int GetMouseHoverIndex();

	/// <summary>
	/// スプライト上にマウスが乗っているか
	/// <param name="sprite">判定対象のスプライトポインタ</param>
	/// <returns>スプライト上にマウスがあればtrue</returns>
	/// </summary>
	bool IsMouseOnSprite(MyEngine::Sprite* sprite);

	/// <summary>
	/// メニュー選択結果を取得
	/// <returns>現在のメニュー選択結果</returns>
	/// </summary>
	MenuResult GetMenuResult() const { return menu_result_; }

private:
	// メニュー選択結果
	MenuResult menu_result_ = MenuResult::None;
	// タイトルのオブジェクト
	std::unique_ptr< MyEngine::Object3d> title_;
	// プレイヤーオブジェクト
	std::unique_ptr< MyEngine::Object3d> player_obj_;
	//スタート項目スプライト
	std::unique_ptr< MyEngine::Sprite> menu_start_;
	//操作説明項目スプライト
	std::unique_ptr< MyEngine::Sprite> menu_howto_;
	//終了項目スプライト
	std::unique_ptr< MyEngine::Sprite> menu_exit_;
	//操作説明スプライト
	std::unique_ptr<MyEngine::Sprite> howto_sprite_;
	// プレイヤーオブジェクトTransform
	Transform player_obj_transform_;
	//メニュー項目の元サイズ保存
	Vector2 start_size_;
	Vector2 howto_size_;
	Vector2 exit_size_;
	// Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	// playerObj回転速度
	const float kRotateSpeed = 0.5f;
    // 入力受付間隔
	const float kInputInterval = 10.0f;
	// ジャンプの速度
	float jump_velocity_ = 0.0f;
	// ジャンプの力
	float jump_power_ = 0.12f;
	// 最大ジャンプ数
	const int kMaxJumpCount = 2;
	// 選択中メニューインデックス
	int select_index_ = 0; 
	// 入力受付間隔タイマー
	float input_timer_ = 0.0f;
	// 重力
	float gravity_ = -0.01f;
	// 地面
	float ground_y_ = -0.5f;
	// ジャンプ間隔
	float jump_interval_ = 300.0f;
	// ジャンプタイマー
	float jump_timer_ = 0.0f;
	// 二段ジャンプ時回転速度
	float jump_rotate_speed_ = 180.0f;
	float howto_scale_ = 0.0f;
	// 選択中メニュー拡縮用タイマー
	float scale_timer_ = 0.0f;
	// ジャンプカウント
	int jump_count_ = 0;
	// ジャンプ中か
	bool is_jumping_ = false;
	// 操作説明表示中か
	bool is_show_howto_ = false;
};