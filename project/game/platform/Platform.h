#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include "ParticleEmitter.h"

// 地面のクラス
class Platform {
public:
	/// <summary>
	/// 初期化を行う
	/// </summary>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// </summary>
	void Update(bool is_start_animation_, bool is_returning_);

	/// <summary>
	/// 描画処理を行う
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示用の更新処理を行う。
	/// </summary>
	void Debug();

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
	// 奥方向移動速度
	float move_speed_z_ = 0.1f;
	//煙パーティクルエミッター
	MyEngine::ParticleEmitter smoke_emitter_;
};
