#pragma once
#include "Pso.h"
#include "DirectXBase.h"
#include "Camera.h"

//3Dオブジェクト共通部
class Object3dBase
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// 必要であれば内部でインスタンスを生成して返す。
	/// </summary>
	static Object3dBase* GetInstance();
	/// <summary>
	/// 初期化を行う。
	/// DirectXBase のポインタを受け取り、内部の描画共通設定や PSO 等を初期化する。
	/// </summary>
	/// <param name="directxBase">描画基盤となる <c>DirectXBase</c> のポインタ。</param>
	void Initialize(DirectXBase*directxBase);
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
public:

	///setter///

	/// <summary>
	/// デフォルトのカメラをセットする。
	/// 描画時に使用する既定カメラを登録する。
	/// </summary>
	/// <param name="camera">設定する <c>Camera</c> のポインタ。</param>
	void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
	///getter///

	/// <summary>
	/// DirectXBase を取得する。
	/// </summary>
	/// <returns>内部で保持している <c>DirectXBase*</c>。</returns>
	DirectXBase* GetDxBase()const { return directxBase_; }
	/// <summary>
	/// デフォルトカメラを取得する。
	/// </summary>
	/// <returns>登録されているデフォルト <c>Camera*</c>（未設定なら nullptr）。</returns>
	Camera* GetDefaultCamera()const { return defaultCamera; }
	/// <summary>
	/// ルートシグネチャを取得する。
	/// </summary>
	/// <returns>内部 PSO が保持する <c>ID3D12RootSignature*</c>。</returns>
	ID3D12RootSignature* GetRootSignature() const {
		return pso_->GetRootSignature();
	}
	/// <summary>
	/// グラフィックスパイプラインステートを取得する。
	/// </summary>
	/// <returns>内部 PSO が保持する <c>ID3D12PipelineState*</c>。</returns>
	ID3D12PipelineState* GetGraphicsPipelineState() const {
		return pso_->GetGraphicsPipelineState();
	}
private:
	/// <summary>コンストラクタ（プライベート：シングルトン用）。</summary>
	Object3dBase() = default;
	/// <summary>デストラクタ（プライベート）。</summary>
	~Object3dBase() = default;
private:
	static Object3dBase* instance;
	Object3dBase* object3dbase_ = nullptr;
	//コピーコンストラクタを無効にする
	Object3dBase(const Object3dBase&) = delete;
	//代入演算子を無効にする
	Object3dBase& operator = (const Object3dBase&) = delete;

	HRESULT hr;
	//DirectXBaseのポインタ
	DirectXBase* directxBase_;
	//Psoのポインタ
	std::unique_ptr<Pso>pso_;
	//Cameraポインタ
	Camera* defaultCamera = nullptr;
};

