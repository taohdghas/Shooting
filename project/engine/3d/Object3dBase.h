#pragma once
#include "DirectXBase.h"
#include "camera.h"
#include "PipeLineStateObject.h"

// 3Dオブジェクト共通部クラス
class Object3dBase {
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// 必要に応じて単一の Object3dBase を返す（生成は内部で行われる場合がある）。
	/// </summary>
	static Object3dBase* GetInstance();

	/// <summary>
	/// 初期化を行う。
	/// DirectXBase のポインタを受け取り、内部の描画共通設定や PSO 等を初期化する。
	/// </summary>
	/// <param name="directx_base">描画基盤となる <c>DirectXBase</c> のポインタ。</param>
	void Initialize(DirectXBase* directx_base);

	/// <summary>
	/// 終了処理を行う。
	/// 内部で確保したリソースやユニークポインタの破棄、状態のクリーンアップを行う。
	/// </summary>
	void Finalize();

	/// <summary>
	/// 共通描画設定を行う。
	/// スプライトやオブジェクト描画前に必要なパイプライン設定やルートシグネチャのバインド等を行う。
	/// </summary>
	void DrawBaseSet();

	/// <summary>
	/// デフォルトのカメラをセットする。
	/// 描画時に使用する既定カメラを登録する。
	/// </summary>
	/// <param name="camera">設定する <c>Camera</c> のポインタ。</param>
	void SetDefaultCamera(Camera* camera) { default_camera_ = camera; }

	/// <summary>
	/// DirectXBase を取得する。
	/// </summary>
	/// <returns>内部で保持している <c>DirectXBase*</c>。</returns>
	DirectXBase* GetDxBase() const { return directx_base_; }

	/// <summary>
	/// デフォルトカメラを取得する。
	/// </summary>
	/// <returns>登録されているデフォルト <c>Camera*</c>（未設定なら nullptr）。</returns>
	Camera* GetDefaultCamera() const { return default_camera_; }

	/// <summary>
	/// ルートシグネチャを取得する。
	/// </summary>
	/// <returns>内部 PSO が保持する <c>ID3D12RootSignature*</c>。</returns>
	ID3D12RootSignature* GetRootSignature() const { return pso_->GetRootSignature(); }

	/// <summary>
	/// グラフィックスパイプラインステートを取得する。
	/// </summary>
	/// <returns>内部 PSO が保持する <c>ID3D12PipelineState*</c>。</returns>
	ID3D12PipelineState* GetGraphicsPipelineState() const { return pso_->GetGraphicsPipelineState(); }

private:
	static Object3dBase* instance;
	// コピーコンストラクタ・代入演算子を削除
	Object3dBase(const Object3dBase&) = delete;
	Object3dBase& operator=(const Object3dBase&) = delete;

	// コンストラクタ・デストラクタはシングルトン用にプライベート
	Object3dBase() = default;
	~Object3dBase() = default;

	DirectXBase* directx_base_ = nullptr;
	std::unique_ptr<PipelineStateObject> pso_;
	Camera* default_camera_ = nullptr;
};

