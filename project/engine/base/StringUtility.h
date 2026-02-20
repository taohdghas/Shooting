#pragma once
#include "Windows.h"
#include <string>

namespace MyEngine {

	//文字コードユーティリティ
	/// <summary>
	/// UTF-8とUTF-16間の文字列変換処理の提供
	/// Windows APIを利用した安全かつ汎用的な文字コード変換の実装
	/// アプリケーション内での文字列エンコーディング変換の共通インターフェースの提供
	/// </summary>
	namespace string_utility {
		/// <summary>
		/// UTF-8 エンコードされた std::string を std::wstring (Windows の UTF-16) に変換する。
		/// </summary>
		std::wstring ConvertString(const std::string& str);
		/// <summary>
		/// std::wstring (Windows の UTF-16) を UTF-8 エンコードされた std::string に変換する。
		/// </summary>
		std::string ConvertString(const std::wstring& str);
	}
}