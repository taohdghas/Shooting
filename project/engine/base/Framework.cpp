#include "Framework.h"
#include "CameraManager.h"

//初期化
void Framework::Initialize() {
    // Windows API関連の初期化
    windowsAPI_ = std::make_unique<WindowsAPI>();
    windowsAPI_->Initialize();

    // DirectXの初期化
    DirectXBase::GetInstance()->Initialize(windowsAPI_.get());

    // 入力管理の初期化
    Input::GetInstance()->Initialize(windowsAPI_.get());

    // 2Dスプライト描画用の初期化
    SpriteBase::GetInstance()->Initialize(DirectXBase::GetInstance());

    // シェーダリソース管理の初期化
    SrvManager::GetInstance()->Initialize(DirectXBase::GetInstance());

    // テクスチャ管理の初期化
    TextureManager::GetInstance()->Initialize(DirectXBase::GetInstance(), SrvManager::GetInstance());

    // 3Dオブジェクト基底クラスの初期化
    Object3dBase::GetInstance()->Initialize(DirectXBase::GetInstance());

    // モデル管理の初期化
    ModelManager::GetInstance()->Initialize(DirectXBase::GetInstance());

    // ImGui (GUI) 管理の初期化
    imguimanager_ = std::make_unique<ImGuiManager>();
    imguimanager_->Initialize(windowsAPI_.get(), DirectXBase::GetInstance(), SrvManager::GetInstance());

    // カメラ管理の初期化
    CameraManager::GetInstance()->Initialize();

    // パーティクル管理の初期化（カメラ参照が必要）
    ParticleManager::GetInstance()->Initialize(DirectXBase::GetInstance(), SrvManager::GetInstance(), camera_.get());

    // シーン管理の取得（シングルトン）
    sceneManager = SceneManager::GetInstance();
}

//終了処理
void Framework::Finalize() {
    // シーン管理の終了
    sceneManager->Finalize();

    // パーティクル管理の終了
    ParticleManager::GetInstance()->Finalize();

    // カメラ管理の終了
    CameraManager::GetInstance()->Finalize();

    // GUI解放
    imguimanager_->Finalize();

    // 3Dオブジェクト基底クラスの終了
    Object3dBase::GetInstance()->Finalize();

    // 3Dモデル管理の終了
    ModelManager::GetInstance()->Finalize();

    // テクスチャ管理の終了
    TextureManager::GetInstance()->Finalize();

    // シェーダリソース管理の終了
    SrvManager::GetInstance()->Finalize();

    // 2Dスプライト管理の終了
    SpriteBase::GetInstance()->Finalize();

    // 入力管理の終了
    Input::GetInstance()->Finalize();

    // DirectX終了処理
    DirectXBase::GetInstance()->Finalize();

    // Windows API終了処理
    windowsAPI_->Finalize();
}

// 毎フレーム更新
void Framework::Update() {

    // Windowsからのメッセージを最優先で処理
    if (windowsAPI_->ProcessMessage()) {
        // 終了リクエストが来たらゲームループを抜ける
        endRequst_ = true;
    }

    // 入力状態の更新
    Input::GetInstance()->Update();

    // 現在のシーンを更新
    sceneManager->Update();
}

// メイン実行関数
void Framework::Run() {
    // ゲーム全体の初期化
    Initialize();

    // メインゲームループ
    while (true) {
        // 毎フレーム更新処理
        Update();

        // 終了リクエストがあればループ終了
        if (IsEndRequst()) {
            break;
        }

        // 描画処理
        Draw();
    }

    // ゲーム全体の終了処理
    Finalize();
}
