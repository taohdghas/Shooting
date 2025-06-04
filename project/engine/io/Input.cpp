#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::instance = nullptr;

//シングルトンインスタンス
Input* Input::GetInstance() {
	if (instance == nullptr) {
		instance = new Input;
	}
	return instance;
}

void Input::Initialize(WindowsAPI*windowsAPI) {
	//借りてきたWinAppのインスタンスを記録
	this->windowsAPI_ = windowsAPI;

	HRESULT result;

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

//終了
void Input::Finalize() {
	delete instance;
	instance = nullptr;
}

void Input::Update() {
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}

//1フレーム入力無視
void Input::ClearInput() {
	memset(key, 0, sizeof(key));
	memset(keyPre, 0, sizeof(keyPre));
}

bool Input::PushKey(BYTE keyNumber) {
	//指定キーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {
	if (keyPre[keyNumber] == 0 && key[keyNumber] != 0) {
		return true;
	}
	return false;
}
