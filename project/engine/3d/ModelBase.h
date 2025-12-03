#pragma once
#include "DirectXBase.h"

//3Dモデル共通部
class ModelBase
{
public:
	/// <summary>
	/// 初期化を行う。
	/// DirectXBase のポインタを受け取り、内部で保持して描画などの共通処理に利用する。
	/// </summary>
	/// <param name="directxBase">描画基盤となる <c>DirectXBase</c> のポインタ。</param>
	void Initialize(DirectXBase* directxBase);

	/// <summary>
	/// 内部で保持している DirectXBase のポインタを取得する。
	/// </summary>
	/// <returns>保持している <c>DirectXBase*</c>。</returns>
	DirectXBase* GetDxBase()const { return directxBase_; }
private:
	DirectXBase* directxBase_;
};

