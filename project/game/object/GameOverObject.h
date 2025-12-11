#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

// ゲームオーバーオブジェクト
class GameOverObject {
public:
    /// <summary>
    /// ゲームオーバーオブジェクトの初期化処理を行う。
    /// </summary>
    void Initialize();

    /// <summary>
    /// ゲームオーバーオブジェクトの毎フレーム更新処理を行う。
    /// </summary>
    void Update();

    /// <summary>
    /// ゲームオーバーオブジェクトの描画処理を行う。
    /// </summary>
    void Draw();

    /// <summary>
    /// ゲームオーバーオブジェクトのデバッグ表示・デバッグ用更新処理を行う
    /// </summary>
    void Debug();

private:
    // 文字構造体
    struct Letter {
        std::unique_ptr< MyEngine::Object3d> obj;
        Transform transform;
        float delay;  // 出現までの遅延時間
    };

    // ゲームオーバー文字のオブジェクト
    std::unique_ptr< MyEngine::Object3d> gameOverObj_;
    // retry のオブジェクト
    std::unique_ptr< MyEngine::Object3d> retryObj_;
    // ゲームオーバーのトランスフォーム
    Transform gameOverTransform_;
    // retry のトランスフォーム
    Transform retryTransform_;
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
};

