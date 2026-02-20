#pragma once
#include "DirectXBase.h"
#include "camera.h"
#include "PipeLineStateObject.h"
#include <memory>

namespace MyEngine {

	// 3Dオブジェクト共通部クラス
    /// <summary>
    ///3Dオブジェクト描画に必要な共通リソースの生成・管理
    ///DirectX基盤との連携・管理
    ///デフォルトカメラの管理・提供
    ///3Dオブジェクト描画時の共通設定処理
    ///シングルトンによるインスタンス管理
    ///初期化・終了処理の提供
    /// </summary>
	class Object3dBase {
	public:
		/// <summary>
		/// シングルトンインスタンスを取得する。
		/// </summary>
		static Object3dBase* GetInstance();

		/// <summary>
		/// 初期化を行う。
		/// </summary>
		void Initialize(DirectXBase* directx_base);

		/// <summary>
		/// 終了処理を行う。
		/// </summary>
		void Finalize();

		/// <summary>
		/// 共通描画設定を行う。
		/// </summary>
		void DrawBaseSet();

		/// <summary>
		/// デフォルトのカメラをセットする。
		/// </summary>
		void SetDefaultCamera(Camera* camera) { default_camera_ = camera; }

		/// <summary>
		/// DirectXBase を取得する。
		/// </summary>
		DirectXBase* GetDxBase() const { return directx_base_; }

		/// <summary>
		/// デフォルトカメラを取得する。
		/// </summary>
		Camera* GetDefaultCamera() const { return default_camera_; }

		/// <summary>
		/// ルートシグネチャを取得する。
		/// </summary>
		ID3D12RootSignature* GetRootSignature() const { return pso_->GetRootSignature(); }

		/// <summary>
		/// グラフィックスパイプラインステートを取得する。
		/// </summary>
		ID3D12PipelineState* GetGraphicsPipelineState() const { return pso_->GetGraphicsPipelineState(); }

	public:
		Object3dBase() = default;
		~Object3dBase() = default;

	private:
		static std::unique_ptr<Object3dBase> instance;
		// コピーコンストラクタ・代入演算子を削除
		Object3dBase(const Object3dBase&) = delete;
		Object3dBase& operator=(const Object3dBase&) = delete;

		DirectXBase* directx_base_ = nullptr;
		std::unique_ptr<PipelineStateObject> pso_;
		Camera* default_camera_ = nullptr;
	};
}