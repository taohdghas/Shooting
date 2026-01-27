#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

// ゲームオーバーオブジェクト
class GameOverObject {
public:

	// メニュー結果
    enum class MenuResult {
        None,
        Retry,
        BackTitle
    };

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

	/// <summary>
	/// スプライト更新
	/// </summary>
    void UpdateSprite();

	/// <summary>
	/// スプライト描画
	/// </summary>
    void DrawSprite();

    /// <summary>
    /// デバッグ表示
    /// </summary>
    void Debug();

    /// <summary>
	/// メニュー結果リセット
    /// </summary>
    void ResetMenuResult() { menu_result_ = MenuResult::None; }

	/// <summary>
	/// メニューインデックス取得
	/// </summary>
    int GetMouseHoverIndex();

	/// <summary>
	/// スプライト上にマウスがあるか判定
	/// </summary>
    bool IsMouseOnSprite(MyEngine::Sprite* sprite);

	/// <summary>
	/// メニュー結果取得
	/// </summary>
    MenuResult GetMenuResult() const { return menu_result_; }

private:
	// メニュー結果
    MenuResult menu_result_ = MenuResult::None;
    // 文字構造体
    struct Letter {
        std::unique_ptr< MyEngine::Object3d> obj;
        Transform transform;
        float delay;  // 出現までの遅延時間
    };
    // ゲームオーバー文字のオブジェクト
    std::unique_ptr< MyEngine::Object3d> game_over_object_;
    //retryのスプライト
	std::unique_ptr< MyEngine::Sprite> retry_sprite_;
    //タイトルへ戻るスプライト
	std::unique_ptr< MyEngine::Sprite> back_title_sprite_;
    // ゲームオーバーのトランスフォーム
    Transform gameOverTransform_;
	//基準サイズ
    Vector2 retry_size_;
    Vector2 back_title_size_;
    // 文字数
    static constexpr int kNumLetters = 8;
    // 文字配列
    std::array<Letter, kNumLetters> letters_;
    // 文字ジャンプ経過時間
    float jumpTimer_ = 0.0f;
    // 現在ジャンプ中の文字インデックス
    int currentIndex_ = 0;
    // 1文字がジャンプする時間
    static constexpr float kJumpDuration = 0.6f;
    // ジャンプ高さ
    static constexpr float kJumpHeight = 0.6f;
    // 次の文字に移るまでの遅延
    static constexpr float kDelayBetween = 0.1f;
    // Δt（固定フレーム時間）
    static constexpr float kDeltaTime = 1.0f / 60.0f;
    // α値
    float alpha_ = 1.0f;
    // α値タイマー
    float alphaTimer_ = 0.0f;
    // 左端の x 座標
    float startX_ = -3.5f;
    // 文字間の間隔
    float spacing_ = 0.9f;
    // 基準座標
    float baseY_ = -1.0f;
	// スケールタイマー
    float scale_timer_ = 0.0f;
	// 選択中インデックス
    int select_index_ = -1;
};

