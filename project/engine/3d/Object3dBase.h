#pragma once
#include "Pso.h"
#include "DirectXBase.h"
#include "Camera.h"

//3Dオブジェクト共通部
class Object3dBase
{
public:
	//シングルトンインスタンス
	static Object3dBase* GetInstance();
	//初期化
	void Initialize(DirectXBase*directxBase);
    //終了
	void Finalize();
	//共通描画設定
	void DrawBaseSet();
public:

	///setter///
	void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
	///getter///
	DirectXBase* GetDxBase()const { return directxBase_; }
	Camera* GetDefaultCamera()const { return defaultCamera; }
	ID3D12RootSignature* GetRootSignature() const {
		return pso_->GetRootSignature();
	}
	ID3D12PipelineState* GetGraphicsPipelineState() const {
		return pso_->GetGraphicsPipelineState();
	}
private:
	//コンストラクタ
	Object3dBase() = default;
	//デストラクタ
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

