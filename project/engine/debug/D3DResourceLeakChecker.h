#pragma once

namespace MyEngine {

	//リークを検出するクラス
	/// <summary>
	/// Direct3D/DirectXリソースのリークの検出
	/// デストラクタでリソースリークの有無をチェックし、検出時はデバッグ出力等で通知
	/// アプリケーション終了時のリソース管理の健全性確認支援
	/// </summary>
	class D3DResourceLeakChecker
	{
	public:
		/// <summary>
        /// デストラクタ
        /// <returns>なし</returns>
        /// </summary>
		~D3DResourceLeakChecker();
	};
}