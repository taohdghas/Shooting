#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include "ParticleEmitter.h"

// 地面のクラス
class Platform {
public:
	/// <summary>
	/// 初期化を行う。
	/// - 引数の <c>Object3dBase*</c> を保持し、内部の <c>Object3d</c> を生成・初期化してモデルや初期トランスフォームを設定する想定。
	/// </summary>
	/// <param name="object3dbase">描画共通設定を提供する <c>Object3dBase*</c>。</param>
	void Initialize(MyEngine::Object3dBase* object3d_base);

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 必要に応じてトランスフォームの同期や物理的な振る舞いを更新する（現状は Transform の反映などを想定）。
	/// </summary>
	void Update(bool is_start_animation_, bool is_returning_);

	/// <summary>
	/// 描画処理を行う。
	/// - 内部の <c>Object3d</c> を用いてプラットフォームを描画する。
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示用の更新処理を行う。
	/// - ImGui 等でパラメータ調整やデバッグ表示を行う実装を想定する。
	/// </summary>
	void Debug();

	/// <summary>
	/// トランスフォーム（平行移動）を取得する。
	/// </summary>
	/// <returns>現在のワールド座標（平行移動）。</returns>
	const Vector3& GetTranslate() const { return transform_.translate; }

	/// <summary>
	/// トランスフォーム（平行移動）を設定する。
	/// </summary>
	/// <param name="translate">設定するワールド座標（平行移動）。</param>
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
