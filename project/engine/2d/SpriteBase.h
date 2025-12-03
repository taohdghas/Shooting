#pragma once
#include "DirectXBase.h"
#include "Pso.h"

//スプライト共通部クラス
class SpriteBase
{
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
	/// <param name="directxBase">描画基盤となる DirectXBase のポインタ。</param>
	void Initialize(DirectXBase*directxBase);
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
public:
	/// <summary>
	/// DirectXBase のポインタを取得する。
	/// </summary>
	/// <returns>内部で保持している DirectXBase*。</returns>
	DirectXBase* GetDxBase()const { return directxBase_; }
private:
	/// <summary>コンストラクタ（プライベート：シングルトン用）。</summary>
	SpriteBase() = default;
	/// <summary>デストラクタ（プライベート）。</summary>
	~SpriteBase() = default;
private:
	static SpriteBase* instance;
	SpriteBase* spritebase_ = nullptr;
	//コピーコンストラクタを無効にする
	SpriteBase(const SpriteBase&) = delete;
	//代入演算子を無効にする
	SpriteBase& operator = (const SpriteBase&) = delete;

	//DirectXBase
	DirectXBase* directxBase_;
	//Pso
	std::unique_ptr<Pso>pso_;
};

