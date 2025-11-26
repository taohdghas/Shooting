#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"
#include "ParticleEmitter.h"

//地面のクラス
class Platform
{
public:
	/// <summary>
	/// 初期化を行う。
	/// - 引数の <c>Object3dBase*</c> を保持し、内部の <c>Object3d</c> を生成・初期化してモデルや初期トランスフォームを設定する想定。
	/// </summary>
	/// <param name="object3dbase">描画共通設定を提供する <c>Object3dBase*</c>。</param>
	void Initialize(Object3dBase*object3dbase);
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 必要に応じてトランスフォームの同期や物理的な振る舞いを更新する（現状は Transform の反映などを想定）。
	/// </summary>
	void Update();
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
public:
	/// <summary>
	/// トランスフォーム（平行移動）を設定する。
	/// </summary>
	/// <param name="translate">設定するワールド座標（平行移動）。</param>
	void SetTranslate(const Vector3& translate) { transform_.translate = translate; }
	/// <summary>
	/// トランスフォーム（平行移動）を取得する。
	/// </summary>
	/// <returns>現在のワールド座標（平行移動）。</returns>
	const Vector3& GetTranslate() const { return transform_.translate; }
private:
	Object3dBase* object3dBase_;
	Transform transform_;
	std::unique_ptr<Object3d>object_;
	ParticleEmitter smokeEmitter_;
};

