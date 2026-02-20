#pragma once
#include "DirectXBase.h"

namespace MyEngine {

	// 3Dモデル共通部クラス
	/// <summary>
	/// 3Dモデル描画に必要な共通リソースの管理
	/// DirectX基盤との連携・管理
	/// 3Dモデル描画処理の基盤となる初期化処理の提供
	/// </summary>
	class ModelBase {
	public:
		/// <summary>
		/// 初期化を行う。
		/// </summary>
		void Initialize(DirectXBase* directx_base);

		/// <summary>
		/// DirectXBase のポインタを取得する。
		/// </summary>
		DirectXBase* GetDxBase() const { return directx_base_; }

	private:
		DirectXBase* directx_base_ = nullptr;
	};
}
