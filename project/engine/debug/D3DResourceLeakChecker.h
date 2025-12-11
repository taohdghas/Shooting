#pragma once

namespace MyEngine {

	//リークを検出するクラス
	class D3DResourceLeakChecker
	{
	public:
		/// <summary>
		/// デストラクタ。
		/// </summary>
		~D3DResourceLeakChecker();
	};
}