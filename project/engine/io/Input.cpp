#include "Input.h"
#include <cassert>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

Input* Input::instance = nullptr;

// シングルトンインスタンス取得
Input* Input::GetInstance() {
    if (instance == nullptr) {
        instance = new Input;
    }
    return instance;
}

void Input::Initialize(WindowsAPI* windowsAPI) {
    // 外部から渡されたWinAPIインスタンスを保持
    this->windowsAPI_ = windowsAPI;

    HRESULT result;

    // DirectInputインスタンス生成
    result = DirectInput8Create(
        windowsAPI->GetHInstance(),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&directInput,
        nullptr
    );
    assert(SUCCEEDED(result));

    // キーボードデバイス生成
    result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
    assert(SUCCEEDED(result));

    // 入力データ形式を設定（標準キーボード形式）
    result = keyboard->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(result));

    // 排他制御レベルの設定（フォアグラウンド＋非排他＋Winキー無効）
    result = keyboard->SetCooperativeLevel(
        windowsAPI->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
    );
    assert(SUCCEEDED(result));

    //カーソル非表示
    ShowCursor(FALSE);

    //マウスデバイス生成
    result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
    assert(SUCCEEDED(result));

    //データ形式を設定
    result = mouse->SetDataFormat(&c_dfDIMouse);
    assert(SUCCEEDED(result));

    //協調レベル設定（フォアグラウンド＋非排他）
    result = mouse->SetCooperativeLevel(
        windowsAPI->GetHwnd(),
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
    );
    assert(SUCCEEDED(result));

    // カーソルをウィンドウ内に制限
    RECT rect;
    GetClientRect(windowsAPI->GetHwnd(), &rect);            
    POINT lt = { rect.left, rect.top };
    POINT rb = { rect.right, rect.bottom };
    ClientToScreen(windowsAPI->GetHwnd(), &lt);             
    ClientToScreen(windowsAPI->GetHwnd(), &rb);
    rect.left = lt.x;
    rect.top = lt.y;
    rect.right = rb.x;
    rect.bottom = rb.y;

    ClipCursor(&rect);  

}

// 終了処理
void Input::Finalize() {
    ShowCursor(TRUE);
    delete instance;
    instance = nullptr;
}

void Input::Update() {
    // 前回のキー入力状態を保存
    memcpy(keyPre, key, sizeof(key));
    // 前回のマウス状態を保存
    mouseStatePre = mouseState;

    // キーボードデバイスから最新の入力状態を取得
    keyboard->Acquire();
    keyboard->GetDeviceState(sizeof(key), key);

    // マウス取得
    mouse->Acquire();
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

}

// 1フレーム分の入力をクリア
void Input::ClearInput() {
    memset(key, 0, sizeof(key));
    memset(keyPre, 0, sizeof(keyPre));
    memset(&mouseState, 0, sizeof(mouseState));
    memset(&mouseStatePre, 0, sizeof(mouseStatePre));
}

// 指定キーが押されていればtrue
bool Input::PushKey(BYTE keyNumber) {
    return key[keyNumber] != 0;
}

// 指定キーが押された瞬間（前回は押されていなかった）ならtrue
bool Input::TriggerKey(BYTE keyNumber) {
    return keyPre[keyNumber] == 0 && key[keyNumber] != 0;
}

// ボタンが押されているか
bool Input::PushMouseButton(int button) {
    if (button < 0 || button > 3) return false; // 左0, 右1, 中2
    return (mouseState.rgbButtons[button] & 0x80) != 0;
}

// トリガー判定
bool Input::TriggerMouseButton(int button) {
    if (button < 0 || button > 3) return false;
    return ((mouseStatePre.rgbButtons[button] & 0x80) == 0) &&
        ((mouseState.rgbButtons[button] & 0x80) != 0);
}

// マウス移動量
int Input::GetMouseMoveX() {
    return mouseState.lX;
}

int Input::GetMouseMoveY() {
    return mouseState.lY;
}
