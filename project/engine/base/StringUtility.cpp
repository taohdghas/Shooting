#include "StringUtility.h"
#include <stringapiset.h>

namespace StringUtility {

    // string -> wstring に変換
    std::wstring ConvertString(const std::string& str) {
        if (str.empty()) {
            // 空文字列はそのまま返す
            return std::wstring();
        }

        // 必要なワイド文字列バッファのサイズを取得
        auto sizeNeeded = MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<const char*>(&str[0]),
            static_cast<int>(str.size()),
            NULL,
            0
        );
        if (sizeNeeded == 0) {
            // 変換に失敗した場合は空文字列を返す
            return std::wstring();
        }

        // 結果を格納する wstring をサイズ分確保
        std::wstring result(sizeNeeded, 0);

        // 実際の変換処理
        MultiByteToWideChar(
            CP_UTF8,
            0,
            reinterpret_cast<const char*>(&str[0]),
            static_cast<int>(str.size()),
            &result[0],
            sizeNeeded
        );

        return result;
    }

    // wstring -> string に変換
    std::string ConvertString(const std::wstring& str) {
        if (str.empty()) {
            return std::string();
        }

        // 必要なマルチバイト文字列バッファのサイズを取得
        auto sizeNeeded = WideCharToMultiByte(
            CP_UTF8,
            0,
            str.data(),
            static_cast<int>(str.size()),
            NULL,
            0,
            NULL,
            NULL
        );
        if (sizeNeeded == 0) {
            // 変換に失敗した場合は空文字列を返す
            return std::string();
        }

        // 結果を格納する string をサイズ分確保
        std::string result(sizeNeeded, 0);

        // 実際の変換処理
        WideCharToMultiByte(
            CP_UTF8,
            0,
            str.data(),
            static_cast<int>(str.size()),
            result.data(),
            sizeNeeded,
            NULL,
            NULL
        );

        return result;
    }

}
