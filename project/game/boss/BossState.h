#pragma once
class Boss1;

//ボスの状態を表す基底クラス
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