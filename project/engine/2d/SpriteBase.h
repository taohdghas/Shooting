#pragma once
#include "DirectXBase.h"
#include "PipelineStateObject.h"
#include <memory>

// スプライト共通部クラス
class SpriteBase {
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// 必要に応じて単一の SpriteBase を返す（生成は内部で行われる場合がある）。
	/// </summary>
	static SpriteBase* GetInstance();

	/// <summary>
	/// 初期化を行う。
	/// DirectXBase の参照を受け取り、内部の描画共通設定や PSO 等を初期化する。
	/// </summary>
	/// <param name="directx_base">描画基盤となる DirectXBase のポインタ。</param>
	void Initialize(DirectXBase* directx_base);

	/// <summary>
	/// 終了処理を行う。
	/// 内部で確保したリソースやユニークポインタの破棄、状態のクリーンアップを行う。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 共通描画設定を行う。
	/// スプライト描画前に必要なパイプライン設定やルートシグネチャのバインド等を行う。
	/// </summary>
	void DrawBaseSet();

	/// <summary>
	/// DirectXBase のポインタを取得する。
	/// </summary>
	/// <returns>内部で保持している DirectXBase*。</returns>
	DirectXBase* GetDxBase() const { return directx_base_; }

public:
	SpriteBase() = default;
	~SpriteBase() = default;

private:
	static std::unique_ptr<SpriteBase> instance;
	// コピーコンストラクタ・代入演算子を削除
	SpriteBase(const SpriteBase&) = delete;
	SpriteBase& operator=(const SpriteBase&) = delete;

	// コンストラクタ・デストラクタはシングルトン用にプライベート
	DirectXBase* directx_base_ = nullptr;
	std::unique_ptr<PipelineStateObject> pso_;
};

