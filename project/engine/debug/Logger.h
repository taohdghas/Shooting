#pragma once
#include "Windows.h"
#include <string>

//ログ出力
namespace Logger
{
	/// <summary>
	/// デバッグ出力にメッセージを送る。
	/// - 実装は <c>OutputDebugStringA</c> を使用し、デバッガ（例: Visual Studio の Output ウィンドウ）に表示されます。
	/// </summary>
	/// <param name="message">出力するメッセージ（std::string）。</param>
	void Log(const std::string& message);
}
