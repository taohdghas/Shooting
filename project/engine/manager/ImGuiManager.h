#pragma once
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "SrvManager.h"
#include <memory>

#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#endif

namespace MyEngine {

    // ImGui を管理するクラス
    /// <summary>
    /// ImGuiの初期化・終了処理の管理
    /// ImGuiフレームの開始・終了・描画処理の制御
    /// DirectXおよびWindowsAPIとの連携によるImGui描画環境の構築
    /// SRV管理との連携
    /// シングルトンによるインスタンス管理
    /// </summary>
    class ImGuiManager {
    public:
        /// <summary>
        /// シングルトンインスタンス
        /// <returns>ImGuiManagerのインスタンス（ポインタ）</returns>
        /// </summary>
        static ImGuiManager* GetInstance();

        /// <summary>
        /// ImGui 関連の終了処理
        /// <returns>なし</returns>
        /// </summary>
        void Finalize();

        /// <summary>
        /// ImGui を初期化
        /// <param name="windows_api">WindowsAPIクラスのポインタ</param>
        /// <param name="directx_base">DirectXBaseクラスのポインタ</param>
        /// <param name="srv_manager">SrvManagerクラスのポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void Initialize(WindowsApi* windows_api, DirectXBase* directx_base, SrvManager* srv_manager);

        /// <summary>
        /// ImGui の受け付けを開始
        /// <returns>なし</returns>
        /// </summary>
        void Begin();

        /// <summary>
        /// ImGui の受け付けを終了
        /// <returns>なし</returns>
        /// </summary>
        void End();

        /// <summary>
        /// 画面への描画
        /// <returns>なし</returns>
        /// </summary>
        void Draw();

    public:
        ImGuiManager() = default;
        ~ImGuiManager() = default;

    private:
        static std::unique_ptr<ImGuiManager> instance_;

        ImGuiManager* imgui_manager_;
        WindowsApi* windows_api_;
        DirectXBase* directx_base_;
        SrvManager* srv_manager_;
        uint32_t index_;
    };
}