#pragma once
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
	//ルートシグネチャの作成
	void GenerateRootSignature();
	//グラフィックスパイプラインの生成
	void GenerategraphicsPipeline();
	//共通描画設定
	void DrawBaseSet();
	///setter///
	void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
	///getter///
	DirectXBase* GetDxBase()const { return directxBase_; }
	Camera* GetDefaultCamera()const { return defaultCamera; }
private:
	static Object3dBase* instance;
	Object3dBase* object3dbase_ = nullptr;

	HRESULT hr;
	//DirectXBaseのポインタ
	DirectXBase* directxBase_;
	//Cameraポインタ
	Camera* defaultCamera = nullptr;
	//ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	//inputlayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	//グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineState;
};

