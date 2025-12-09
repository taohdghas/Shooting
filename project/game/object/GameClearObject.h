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
    //文字構造体
    struct Letter {
        std::unique_ptr< MyEngine::Object3d> object;
        Transform transform;
        float delay; // 出現までの遅延時間
    };

    // ゲームクリア文字のオブジェクト
    std::unique_ptr< MyEngine::Object3d> game_clear_object_;
    // push spaceのオブジェクト
    std::unique_ptr< MyEngine::Object3d> push_space_object_;
    // ゲームクリアトランスフォーム
    Transform game_clear_transform_;
    // push spaceトランスフォーム
    Transform push_space_transform_;
    // 文字数
    static const int kNumLetters = 9;
    // 文字配列
    std::array<Letter, kNumLetters> letters_;
    // 文字ジャンプ経過時間
    float jump_timer_ = 0.0f;
    // 現在ジャンプ中の文字インデックス
    int current_index_ = 0;
    // 1文字がジャンプする時間
    const float kJumpDuration = 0.5f;
    // ジャンプ高さ
    const float kJumpHeight = 0.6f;
    // 次の文字に移るまでの遅延
    const float kDelayBetween = 0.1f;
    // Δt
    const float kDeltaTime = 1.0f / 60.0f;
    // α値
    float alpha_ = 1.0f;
    // α値タイマー
    float alpha_timer_ = 0.0f;
    // 左端のx座標
    float kStartX = -3.5f;
    // 文字間の間隔
    float kSpacing = 0.8f;
    // 基準y座標
    float kBaseY = 0.2f;
};