#pragma once
#include <d3d12.h>
#include <wrl.h>

//DirectX基盤クラス前方宣言
class DirectXBase;
//パイプラインステートを管理するクラス
class Pso
{
public:
	//初期化
	void Initialize(DirectXBase*directXBase);
	//Object3D用RootSignature
	void CreateRootSignature();
	//Object3D用PipelineState
	void CreatePipelineState();
	//Sprite用RootSignature
	void CreateSpriteRootSignature();
	//Sprite用PipelineState
	void CreateSpritePipelineState();
	//Particle用RootSignature
	void CreateParticleRootSignature();
	//Particle用PipelineState
	void CreateParticlePipelineState();
	//PostEffect用RootSignature
	void CreatePostEffectRootSignature();
	//PostEffect用PipelineState
	void CreatePostEffectPipelineState();
	//SkyBox用RootSignature
	void CreateSkyBoxRootSignature();
	//SkyBox用PipelineState
	void CreateSkyBoxPipelineState();
public:
	///Getter///
	//Object3D用
	ID3D12RootSignature* GetRootSignature()const { return rootSignature.Get(); }
	ID3D12PipelineState* GetGraphicsPipelineState()const { return graphicsPipelineState.Get(); }
	//Sprite用
	ID3D12RootSignature* GetSpriteRootSignature()const { return spriteRootSignature.Get(); }
	ID3D12PipelineState* GetSpriteGraphicsPipelineState()const { return spriteGraphicsPipelineState.Get(); }
	//Particle用
	ID3D12RootSignature* GetParticleRootSignature()const { return particleRootSignature.Get(); }
	ID3D12PipelineState* GetParticleGraphicsPipelineState()const { return particleGraphicsPipelineState.Get(); }
	//PostEffect用
	ID3D12RootSignature* GetPostEffectRootSignature()const { return postEffectRootSignature.Get(); }
	ID3D12PipelineState* GetPostEffectGraphicsPipelineState()const { return postEffectGraphicsPipelineState.Get(); }
	//SkyBox用
	ID3D12RootSignature* GetSkyBoxRootSignature()const { return skyBoxRootSignature.Get(); }
	ID3D12PipelineState* GetSkyBoxGraphicsPipelineState()const { return skyBoxGraphicsPipelineState.Get(); }

private:
	DirectXBase* directXBase_;
	//Object3D用
	Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>graphicsPipelineState;
	//Sprite用
	Microsoft::WRL::ComPtr<ID3D12RootSignature>spriteRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>spriteGraphicsPipelineState;
	//Particle用
	Microsoft::WRL::ComPtr<ID3D12RootSignature>particleRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>particleGraphicsPipelineState;
	//PostEffect用
	Microsoft::WRL::ComPtr<ID3D12RootSignature>postEffectRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>postEffectGraphicsPipelineState;
	//Skybox用
	Microsoft::WRL::ComPtr<ID3D12RootSignature>skyBoxRootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>skyBoxGraphicsPipelineState;

};

