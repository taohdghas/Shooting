#pragma once
#include "D3DResourceLeakChecker.h"
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "Input.h"
#include "SpriteBase.h"
#include "SrvManager.h"
#include "Object3dBase.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "ParticleManager.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"

//アプリケーション全体の実行を管理する基底クラス
class Framework
{
public:
	/// <summary>
	/// 初期化処理を行う。
	/// - WindowsAPI の初期化
	/// - DirectXBase / Input / SpriteBase / SrvManager / TextureManager / Object3dBase / ModelManager の順で各種マネージャーや共通部を初期化
	/// - ImGuiManager の生成と初期化
	/// - CameraManager, ParticleManager の初期化
	/// - SceneManager の取得と設定
	/// </summary>
	virtual void Initialize();
    /// <summary>
	/// 終了処理を行う。
	/// - SceneManager, ParticleManager, CameraManager, ImGuiManager など各種モジュールを逆順に破棄・終了してリソースを解放する。
	/// </summary>
	virtual void Finalize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - Windows のメッセージを優先で処理し、終了要求があれば内部フラグを立てる
	/// - 入力の更新、シーンマネージャの更新を行う
	/// </summary>
	virtual void Update();
	/// <summary>
	/// 毎フレームの描画処理（派生クラスで実装必須）。
	/// - ゲームループ内で呼ばれ、フレームごとの描画コマンドを発行する責務を持つ（純粋仮想）。
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 終了要求フラグを取得する。
	/// </summary>
	/// <returns>終了リクエストがある場合は true、それ以外は false を返す。</returns>
	virtual bool IsEndRequst() { return endRequst_; }
    /// <summary>
	/// デストラクタ（派生クラスでのクリーンアップを許容）。
	/// </summary>
	virtual ~Framework() = default;
public:
	/// <summary>
	/// アプリケーションを実行する。
	/// - Initialize() を呼び、ゲームループ（Update() と Draw()）を回し、終了後に Finalize() を呼ぶ。
	/// - ループは Update() 内でのメッセージ処理により終了要求が立つと抜ける。
	/// </summary>
	void Run();
protected:
	D3DResourceLeakChecker leakCheck;
	//WindowsAPIポインタ
	std::unique_ptr<WindowsAPI>windowsAPI_;
	//ImGuiマネージャ
	std::unique_ptr<ImGuiManager>imguimanager_;
	//カメラ
	std::unique_ptr<Camera>camera_;
	//シーンマネージャ
	SceneManager*sceneManager;
	//シーンファクトリー
	std::unique_ptr<AbstractSceneFactory>sceneFactory;
	//ゲーム終了フラグ
	bool endRequst_ = false;
};

