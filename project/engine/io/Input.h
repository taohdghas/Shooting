#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Windows.h"
#include <dinput.h>
#include <wrl.h>
#include "WindowsAPI.h"
class Input
{
public:
	//namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//シングルトンインスタンス
	static Input* GetInstance();
	//初期化
	void Initialize(WindowsAPI*windowsAPI);
	//終了
	void Finalize();
	//更新
	void Update();
	//1フレーム入力無視
	void ClearInput();

/// <summary>
/// キーの押下をチェック
/// </summary>
/// <param name = "keyNumber">キー番号(DIK_0等)</param>
/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
/// キーのトリガーをチェック
/// </summary>
/// <param name = "keyNumber">キー番号(DIK_0等)</param>
/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);
private:
	static Input* instance;
	Input* input_ = nullptr;

	//キーボードのデバイス
	ComPtr<IDirectInputDevice8>keyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回のキーの状態
	BYTE keyPre[256] = {};
	//DirectInputのインスタンス
	ComPtr<IDirectInput8>directInput;
	//WindowsAPI
	WindowsAPI* windowsAPI_ = nullptr;
};

