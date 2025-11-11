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
        std::unique_ptr<Object3d> obj;
        Transform transform;
        float delay;   //出現までの遅延時間
    };

    //ゲームクリア字のオブジェクト
    std::unique_ptr<Object3d>gameclear;
    //pushspaceのオブジェクト
    std::unique_ptr<Object3d>pushspace;
    //ゲームクリアトランスフォーム
    Transform gameclearTransform;
    //nextトランスフォーム
    Transform pushspaceTransform;
    //文字数
    static const int NumLetters = 9; 
    //文字配列
    std::array<Letter, NumLetters> letters;
    //文字ジャンプ経過時間
    float jumpTimer_ = 0.0f; 
    //現在ジャンプ中の文字インデックス
    int currentIndex_ = 0;       
    //1文字がジャンプする時間
    const float jumpDuration_ = 0.6f;
    //ジャンプ高さ
    const float jumpHeight_ = 0.6f; 
    //次の文字に移るまでの遅延
    const float delayBetween_ = 0.1f;
    //Δtを定義
    const float DeltaTime = 1.0f / 60.0f;
    //α値
    float alpha_ = 1.0f;
    //α値タイマー
    float alphaTimer_ = 0.0f;
    //左端のx座標
    float startX = -3.5f;
    //文字間の間隔
    float spacing = 0.8f; 
    //正気座標
    float baseY = -1.0f; 
};
