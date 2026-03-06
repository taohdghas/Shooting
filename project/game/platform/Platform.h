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
	/// 初期化を行う
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update(bool is_start_animation_, bool is_returning_,bool is_control_enabled_);

	/// <summary>
	/// 描画処理を行う
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示用の更新処理を行う。
	/// </summary>
	void Debug();

	/// <summary>
	/// 移動を停止する。
	/// </summary>
	void Stop() { is_stopped_ = true; }

	/// <summary>
	/// 移動が停止しているかを取得する。
	/// </summary>
	bool IsStopped() const { return is_stopped_; }

	/// <summary>
	/// トランスフォーム（平行移動）を取得する。
	/// </summary>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// トランスフォーム（平行移動）を設定する。
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