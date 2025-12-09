#pragma once
#include "DirectXBase.h"

namespace MyEngine {

	// 3Dモデル共通部クラス
	class ModelBase {
	public:
		/// <summary>
		/// 初期化を行う。
		/// DirectXBase の参照を受け取り、モデル描画に必要な基盤設定を行う。
		/// </summary>
		/// <param name="directx_base">描画基盤となる DirectXBase のポインタ。</param>
		void Initialize(DirectXBase* directx_base);

		/// <summary>
		/// DirectXBase のポインタを取得する。
		/// </summary>
		/// <returns>内部で保持している DirectXBase*。</returns>
		DirectXBase* GetDxBase() const { return directx_base_; }

	private:
		DirectXBase* directx_base_ = nullptr;
	};
}
