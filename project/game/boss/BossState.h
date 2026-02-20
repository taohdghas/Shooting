#pragma once
class Boss1;

//ボスの状態を表す基底クラス
/// <summary>
///ボスの状態ごとの振る舞いを抽象化し、状態遷移を管理する基盤を提供
///状態ごとに「開始処理」「毎フレーム更新」「終了処理」のインターフェースを定義
///各具体的なボス状態の共通基底として機能
///状態遷移時のボス本体への操作・制御の受け渡し
///派生クラスで状態固有のロジックを実装するための抽象クラス
/// </summary>
class BossState {
public:
    /// <summary>
	/// デストラクタ 
    /// </summary>
    virtual ~BossState() = default;
    /// <summary>
	/// 状態に入るときの処理
    /// </summary>
    virtual void Enter(Boss1& boss) {}
    /// <summary>
	/// 状態の更新
    /// </summary>
    virtual void Update(Boss1& boss) = 0;
    /// <summary>
	/// 状態が終了
    /// </summary>
    virtual void Exit(Boss1& boss) {}
};