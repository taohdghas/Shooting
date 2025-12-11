#pragma once
#include "DirectXBase.h"
#include "PipelineStateObject.h"
#include <memory>

namespace MyEngine {

	// スプライト共通部クラス
	class SpriteBase {
	public:
		/// <summary>
		/// シングルトンインスタンスを取得する。
		/// </summary>
		static SpriteBase* GetInstance();

		/// <summary>
		/// 初期化を行う。
		/// </summary>
		void Initialize(DirectXBase* directx_base);

		/// <summary>
		/// 終了処理を行う。
		/// </summary>
		void Finalize();

		/// <summary>
		/// 共通描画設定を行う。
		/// </summary>
		void DrawBaseSet();

		/// <summary>
		/// DirectXBase のポインタを取得する。
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
