#pragma once
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "SrvManager.h"

#ifdef USE_IMGUI
#include "externals//imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#endif

class ImGuiManager
{
public:
	//シングルトンインスタンス
	static ImGuiManager* GetInstance();
	//終了
	void Finalize();
	//初期化
	void Initialize(WindowsAPI*windowsAPI,DirectXBase*directxBase,SrvManager*srvmanager);
	//ImGui受付開始
	void Begin();
	//ImGui受付終了
	void End();
	//画面への描画
	void Draw();
private:
	static ImGuiManager* instance;
	ImGuiManager* imguiManager_;
	WindowsAPI* windowsAPI_;
	DirectXBase* directxbase_;
	SrvManager* srvmanager_;
	uint32_t index;
};

