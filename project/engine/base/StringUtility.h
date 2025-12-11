#pragma once
#include "Windows.h"
#include <string>

namespace MyEngine {

	//文字コードユーティリティ
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