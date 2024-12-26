#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::instance = nullptr;
//インスタンスの取得
Input* Input::GetInstance() {
	if (instance == nullptr) {
		instance = new Input;
	}
	return instance;
}

void Input::Finalize() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

void Input::Initialize(WindowsAPI*windowsAPI) {
	HRESULT result;
	//借りてきたWinAppのインスタンスを記録
	windowsAPI_ = windowsAPI;

	//DirectInputのインスタンス生成
	result = DirectInput8Create(windowsAPI->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイス生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));
	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));
	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(windowsAPI->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Input::Update() {
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber) {
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {

	if (keyPre[keyNumber]) {
		return true;
	}
	return false;
}
