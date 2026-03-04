#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include "ParticleEmitter.h"

// 地面のクラス
/// <summary>
/// 地面オブジェクトの状態管理
/// 3Dオブジェクトの初期化・更新・描画処理の実装
/// プレイヤーや他オブジェクトとの連携のための座標取得・設定インターフェースの提供
/// 移動制御やデバッグ表示の実装
/// 煙パーティクルエミッター等の演出リソース管理
/// </summary>
class Platform {
public:
	/// <summary>
	/// 初期化
	/// <param name="object3d_base">3Dオブジェクト共通設定へのポインタ</param>
	/// <returns>なし</returns>
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 更新
	/// <param name="is_start_animation_">開始アニメーション中か</param>
	/// <param name="is_returning_">リターン中か</param>
	/// <returns>なし</returns>
	/// </summary>
	void Update(bool is_start_animation_, bool is_returning_);

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// <returns>なし</returns>
	/// </summary>
	void Debug();

	/// <summary>
	/// 移動を停止
	/// <returns>なし</returns>
	/// </summary>
	void Stop() { is_stopped_ = true; }

	/// <summary>
	/// 移動が停止しているかを取得
	/// <returns>停止していればtrue</returns>
	/// </summary>
	bool IsStopped() const { return is_stopped_; }

	/// <summary>
	/// トランスフォームを取得
	/// <returns>現在の座標（Vector3参照）</returns>
	/// </summary>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// トランスフォームを設定
	/// <param name="translate">設定する座標値</param>
	/// <returns>なし</returns>
	/// </summary>
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }

private:
	MyEngine::Object3dBase* object3d_base_;
	Transform transform_;
	std::unique_ptr< MyEngine::Object3d> object_;
	//煙パーティクルエミッター
	MyEngine::ParticleEmitter smoke_emitter_;
	// 奥方向移動速度
	float move_speed_z_ = 0.1f;
	// 停止フラグ
	bool is_stopped_ = false;

};