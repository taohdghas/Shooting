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

namespace MyEngine {

    //アプリケーション全体の実行を管理する基底クラス
    /// <summary>
    ///アプリケーション全体のライフサイクルの管理
    ///ウィンドウ、DirectX、ImGui、カメラ、シーンなど主要システムの初期化・解放・連携
    ///シーン管理の基盤提供
    ///ゲームループによるフレーム制御と終了判定
    ///派生クラスでの描画処理の実装要求（純粋仮想関数）
    /// </summary>
    class Framework
    {
    public:
        /// <summary>
        /// 初期化処理を行う。
        /// </summary>
        virtual void Initialize();
        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        virtual void Finalize();
        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// </summary>
        virtual void Update();
        /// <summary>
        /// 毎フレームの描画処理（派生クラスで実装必須）。
        /// </summary>
        virtual void Draw() = 0;
        /// <summary>
        /// 終了要求フラグを取得する。
        /// </summary>
        virtual bool is_end_request() { return end_request_; }
        /// <summary>
        /// デストラクタ（派生クラスでのクリーンアップを許容）。
        /// </summary>
        virtual ~Framework() = default;

    public:
        /// <summary>
        /// アプリケーションを実行する。
        /// </summary>
        void Run();

    protected:
        D3DResourceLeakChecker leak_check_;
        // WindowsAPIポインタ
        std::unique_ptr<WindowsApi> windows_api_;
        // ImGuiマネージャ
        std::unique_ptr<ImGuiManager> imgui_manager_;
        // カメラ
        std::unique_ptr<Camera> camera_;
        // シーンマネージャ
        SceneManager* scene_manager_;
        // シーンファクトリー
        std::unique_ptr<AbstractSceneFactory> scene_factory_;
        // ゲーム終了フラグ
        bool end_request_ = false;
    };
}