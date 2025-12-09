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
    class MyGame : public Framework {
    public:
        /// <summary>
        /// ゲームの初期化を行う。
        /// - 基底クラス `Framework::Initialize()` を呼び、各種サブシステムを初期化する。
        /// - `SceneFactory` を生成して `SceneManager` にセットし、最初のシーンとして "TITLE" を設定する。
        /// </summary>
        void Initialize() override;

        /// <summary>
        /// 終了処理を行う。
        /// - 基底クラス `Framework::Finalize()` を呼んで各種リソースを解放する。
        /// </summary>
        void Finalize() override;

        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// - ImGui の受け付けを開始・終了する。
        /// - 基底クラス `Framework::Update()` を呼んでメッセージ処理・入力・シーン更新等を行う。
        /// - `USE_IMGUI` が定義されている場合は ImGui ウィンドウ上で `use_render_texture_` の切り替えを行える UI を表示する。
        /// </summary>
        void Update() override;

        /// <summary>
        /// 描画処理を行う。
        /// - `use_render_texture_` に応じてレンダーターゲットを切り替え、
        ///   シーン描画 → 必要ならレンダーテクスチャを SRV に遷移 → スワップチェインへ描画、という流れを実行する。
        /// - いずれの場合も `SrvManager::PreDraw()` と `ImGuiManager::Draw()` を適切なタイミングで呼ぶ。
        /// </summary>
        void Draw() override;

    private:
        // RenderTexture切り替え
        bool use_render_texture_ = false;
    };
}