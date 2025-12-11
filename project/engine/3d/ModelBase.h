#pragma once
#include "DirectXBase.h"

namespace MyEngine {

	// 3Dモデル共通部クラス
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
