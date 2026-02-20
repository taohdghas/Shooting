#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <vector>
#include <numbers>
#include <fstream>
#include <sstream>
#include <dinput.h>
#include "MyMath.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "Struct.h"
#include "Framework.h"

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include "Logger.h"
using namespace Math;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dinput8.lib")

namespace MyEngine {

    // ゲーム全体を司るクラス
    /// <summary>
    /// ゲームアプリケーション全体の初期化、更新、描画、終了処理の実装
    /// ゲーム固有のリソースやシーン、レンダリング設定の管理
    /// Framework基底クラスから継承したライフサイクル制御の具体的なゲーム処理への適用
    /// ゲーム全体の状態管理やフレームごとの制御
    /// </summary>
    class MyGame : public Framework {
    public:
        /// <summary>
        /// ゲームの初期化を行う。
        /// </summary>
        void Initialize() override;

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize() override;

        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// </summary>
        void Update() override;

        /// <summary>
        /// 描画処理を行う。
        /// </summary>
        void Draw() override;

    private:
        // RenderTexture切り替え
        bool use_render_texture_ = false;
    };
}