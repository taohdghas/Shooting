#pragma once
#include "DirectXBase.h"
#include "PipelineStateObject.h"
#include <memory>

namespace MyEngine {

	// スプライト共通部クラス
	/// <summary>
	/// SpriteBaseクラスの責務
	/// スプライト描画に必要な共通リソースの生成・管理
	/// DirectX基盤との連携・管理
	/// スプライト描画時の共通設定処理
	/// シングルトンによるインスタンス管理
	/// 初期化・終了処理
	/// </summary>
	class SpriteBase {
	public:
		/// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>SpriteBaseのインスタンス（ポインタ）</returns>
        /// </summary>
		static SpriteBase* GetInstance();

		/// <summary>
		/// 初期化を行う。
		/// <param name="directx_base">DirectX基盤クラスのポインタ</param>
		/// <returns>なし</returns>
		/// </summary>
		void Initialize(DirectXBase* directx_base);

		/// <summary>
		/// 終了処理
		/// <returns>なし</returns>
		/// </summary>
		void Finalize();

		/// <summary>
		/// 共通描画設定
		/// <returns>なし</returns>
		/// </summary>
		void DrawBaseSet();

		/// <summary>
		/// DirectXBase のポインタを取得
		/// <returns>DirectXBaseのポインタ</returns>
		/// </summary>
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
}