#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Windows.h"
#include <dinput.h>
#include <wrl.h>
#include "WindowsAPI.h"

//入力関連クラス
class Input
{
public:
	//namespace省略
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// - 初回呼び出し時に内部でインスタンスを生成して返します。
	/// </summary>
	/// <returns>Input の単一インスタンスへのポインタ。</returns>
	static Input* GetInstance();
	/// <summary>
	/// 初期化を行う。
	/// - 引数で渡された WindowsAPI から HINSTANCE/HWND を取得し DirectInput を初期化する。
	/// - キーボードデバイスを生成し、データフォーマットの設定と協調レベル（DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY）を行う。
	/// </summary>
	/// <param name="windowsAPI">ウィンドウ情報を提供する WindowsAPI のポインタ。</param>
	void Initialize(WindowsAPI*windowsAPI);
	/// <summary>
	/// 終了処理を行う。
	/// - シングルトンインスタンスを破棄します（内部で delete を呼ぶ実装を想定）。
	/// </summary>
	void Finalize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 前フレームのキー状態を保存し、キーボードデバイスから現在のキー状態を取得して保持する。
	/// </summary>
	void Update();
	/// <summary>
	/// 1フレーム分の入力をクリアする（入力を無視する）。
	/// - 内部のキー状態配列をゼロクリアする実装。
	/// </summary>
	void ClearInput();

	/// <summary>
	/// 指定キーが押されているかをチェックする。
	/// </summary>
	/// <param name="keyNumber">チェックするキー番号（DIK_* の値）。</param>
	/// <returns>押下中であれば true、そうでなければ false を返す。</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// 指定キーがトリガー（前フレームは未押下、今回押下）かをチェックする。
	/// </summary>
	/// <param name="keyNumber">チェックするキー番号（DIK_* の値）。</param>
	/// <returns>トリガーであれば true、そうでなければ false を返す。</returns>
	bool TriggerKey(BYTE keyNumber);

	bool PushMouseButton(int button);   // 押されているか
	bool TriggerMouseButton(int button); // トリガーか
	int GetMouseMoveX();                 // X方向の移動量
	int GetMouseMoveY(); 

private:
	static Input* instance;
	Input* input_ = nullptr;
	//WindowsAPI
	WindowsAPI* windowsAPI_ = nullptr;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8>keyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回のキーの状態
	BYTE keyPre[256] = {};
	//DirectInputのインスタンス
	ComPtr<IDirectInput8>directInput;
	// マウス関連
	ComPtr<IDirectInputDevice8> mouse;
	//現在のマウス状態
	DIMOUSESTATE mouseState{};    
	//前のフレーム
	DIMOUSESTATE mouseStatePre{};  
};

