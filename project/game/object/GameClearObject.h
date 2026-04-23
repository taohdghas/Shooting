#pragma once
#include "Sprite.h"
#include "SpriteBase.h"
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

//ゲームクリアオブジェクト
/// <summary>
/// ゲームクリア画面の表示・演出
///「PUSH SPACE」等の案内スプライトの管理と描画
/// スプライトや3Dオブジェクトの初期化・更新・描画処理
/// 文字列オブジェクトのアニメーション制御
/// 演出パラメータの管理
/// デバッグ表示や状態管理の補助機能の提供
/// </summary>
class GameClearObject
{
public:
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
    /// オブジェクトの描画
    /// <returns>なし</returns>
    /// </summary>
    void Draw();

	/// <summary>
	/// スプライトの描画
	/// <returns>なし</returns>
	/// </summary>
	void DrawSprite();

    /// <summary>
    /// デバッグ
    /// <returns>なし</returns>
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
	//push spaceスプライト
	std::unique_ptr< MyEngine::Sprite> push_space_sprite_;
    // ゲームクリアトランスフォーム
    Transform game_clear_transform_;
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
	// π
    const float kPi = 3.1415926535f;
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