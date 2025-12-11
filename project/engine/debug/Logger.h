#pragma once
#include "Windows.h"
#include <string>

namespace MyEngine {

	//ログ出力
	namespace Logger
	{
		/// <summary>
		/// デバッグ出力にメッセージを送る。
		/// </summary>
		void Log(const std::string& message);
	}
}