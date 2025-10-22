#pragma once
#include "Windows.h"
#include <string>

//文字コードユーティリティ
namespace StringUtility {
	/// <summary>
	/// UTF-8 エンコードされた std::string を std::wstring (Windows の UTF-16) に変換する。
	/// </summary>
	/// <param name="str">変換元の UTF-8 エンコードされた文字列。</param>
	/// <returns>変換後の std::wstring（UTF-16）。</returns>
	std::wstring ConvertString(const std::string& str);
	/// <summary>
	/// std::wstring (Windows の UTF-16) を UTF-8 エンコードされた std::string に変換する。
	/// </summary>
	/// <param name="str">変換元の std::wstring（UTF-16）。</param>
	/// <returns>変換後の UTF-8 エンコードされた std::string。</returns>
	std::string ConvertString(const std::wstring& str);
}
