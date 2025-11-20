#pragma once
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "SrvManager.h"

#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#endif

// ImGui を管理するクラス
class ImGuiManager {
public:
    // シングルトンインスタンス
    /// <summary>
    /// ImGuiManager の単一インスタンスを取得する。
    /// - 初回呼び出し時に内部でインスタンスを生成して返します。
    /// </summary>
    static ImGuiManager* GetInstance();

    // 終了
    /// <summary>
    /// ImGui 関連の終了処理を行う。
    /// - DirectX と Win32 用の ImGui バックエンドをシャットダウンし、ImGui コンテキストを破棄します。
    /// - シングルトンインスタンスを delete して無効化します。
    /// </summary>
    void Finalize();

    // 初期化
    /// <summary>
    /// ImGui を初期化する。
    /// - 引数で受け取った WindowsAPI / DirectXBase / SrvManager を内部に保持します。
    /// - SrvManager からデスクリプタを割り当て、ImGui コンテキストを生成してスタイルを設定します。
    /// - 日本語フォントを読み込み、Win32 および DirectX12 用の ImGui 実装を初期化します。
    /// </summary>
    /// <param name="windows_api">Win32 ウィンドウ情報を提供する WindowsApi*</param>
    /// <param name="directx_base">DirectX リソースを管理する DirectXBase*</param>
    /// <param name="srv_manager">SRV の割り当ておよびデスクリプタ情報を提供する SrvManager*</param>
    void Initialize(WindowsApi* windows_api, DirectXBase* directx_base, SrvManager* srv_manager);

    // ImGui 受付開始
    /// <summary>
    /// ImGui の受け付けを開始するフレーム処理。
    /// - DX12 と Win32 の各フレーム開始処理を呼び出し、ImGui の新規フレームを開始します。
    /// </summary>
    void Begin();

    // ImGui 受付終了
    /// <summary>
    /// ImGui の受け付けを終了する処理。
    /// - 描画前準備として ImGui::Render() を呼び、描画データを確定します（描画自体は Draw() で実行）。
    /// </summary>
    void End();

    // 画面への描画
    /// <summary>
    /// 確定された ImGui の描画データを GPU に送って描画する。
    /// - DirectX のコマンドリストを取得し、SrvManager のデスクリプタヒープをセットした上で
    ///   ImGui_ImplDX12_RenderDrawData を呼んで描画コマンドを発行します。
    /// </summary>
    void Draw();

private:
    static ImGuiManager* instance_;

    ImGuiManager* imgui_manager_;
    WindowsApi* windows_api_;
    DirectXBase* directx_base_;
    SrvManager* srv_manager_;
    uint32_t index_;
};
