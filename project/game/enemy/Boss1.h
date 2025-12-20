#pragma once
#include "EnemyBase.h"
#include "EnemyBullet.h"

// ボス敵クラス
class Boss1 : public EnemyBase {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base) override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug(int id) override;

	/// <summary>
	/// ボス用弾リスト取得
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

private:
	// ボス用弾リスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

};
