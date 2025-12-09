#include "Logger.h"
#include <debugapi.h>

namespace MyEngine {

    namespace Logger {

        // デバッグ用に文字列を出力
        void Log(const std::string& message) {
            // Visual Studioのデバッグ出力に送る
            OutputDebugStringA(message.c_str());
        }

    }
}