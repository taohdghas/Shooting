#pragma once
#include <Windows.h>
#include <cstdint>
class WindowsAPI
{
public:
	//静的メンバ関数
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public:
	//シングルトンインスタンス
	static WindowsAPI* GetInstance();
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//メッセージの処理
	bool ProcessMessage();
	//getter
	HWND GetHwnd()const { return hwnd;}
	HINSTANCE GetHInstance()const { return wc.hInstance; }
	//クライアント領域のサイズ
	static const int32_t kClientWitdh = 1280;
	static const int32_t kClientHeight = 720;
private:
	static WindowsAPI* instance;
	WindowsAPI* windowsAPI_ = nullptr;
	//ウインドウハンドル
	HWND hwnd = nullptr;
	//ウインドウクラスの設定
	WNDCLASS wc{};
};

