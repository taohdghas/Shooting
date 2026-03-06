#pragma once
#include "Windows.h"
#include <string>

namespace MyEngine {

	//ログ出力
	/// <summary>
	/// アプリケーション全体のデバッグ・情報・エラー等のログメッセージ出力処理の提供
	/// Windowsのデバッグ出力を利用したメッセージ出力
	/// 文字列メッセージの一元的なログインターフェースの提供
	/// 開発・デバッグ時の情報可視化支援
	/// </summary>
	namespace Logger
	{
		/// <summary>
        /// デバッグ出力にメッセージを送る
        /// <param name="message">出力するメッセージ文字列</param>
        /// <returns>なし</returns>
        /// </summary>
		void Log(const std::string& message);
	}
}