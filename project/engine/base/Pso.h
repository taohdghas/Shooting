#pragma once
#include <d3d12.h>
#include <wrl.h>

//DirectX基盤クラス前方宣言
class DirectXBase;
//パイプラインステートを管理するクラス
class Pso
{
public:
	/// <summary>
	/// 初期化する。
	/// DirectXBase のポインタを保存し、以降の RootSignature / PipelineState 作成で利用する。
	/// </summary>
	/// <param name="directXBase">DirectX の共通処理を提供する <c>DirectXBase</c> のポインタ。</param>
	void Initialize(DirectXBase*directXBase);
	/// <summary>
	/// Object3D 用のルートシグネチャを作成する。
	/// - CBV / SRV のルートパラメータ、スタティックサンプラを設定し、シリアライズした後にデバイス上で生成する。
	/// </summary>
	void CreateRootSignature();
	/// <summary>
	/// Object3D 用のグラフィックスパイプラインステートを作成する。
	/// - 頂点レイアウト、ブレンド／ラスタライザ／深度ステンシル等を設定し、シェーダーをコンパイルして PSO を生成する。
	/// </summary>
	void CreatePipelineState();
	/// <summary>
	/// Sprite 用のルートシグネチャを作成する。
	/// - Sprite 固有の SRV/CBV レイアウトとスタティックサンプラを定義して生成する。
	/// </summary>
	void CreateSpriteRootSignature();
	/// <summary>
	/// Sprite 用のグラフィックスパイプラインステートを作成する。
	/// - Sprite シェーダーをコンパイルし、入力レイアウトやブレンドなどを設定して PSO を生成する。
	/// </summary>
	void CreateSpritePipelineState();
	/// <summary>
	/// Particle 用のルートシグネチャを作成する。
	/// - インスタンシング用の SRV テーブルや必要な CBV を定義して生成する。
	/// </summary>
	void CreateParticleRootSignature();
	/// <summary>
	/// Particle 用のグラフィックスパイプラインステートを作成する。
	/// - パーティクル描画に適したブレンド設定や深度設定を行い、対応シェーダーで PSO を生成する。
	/// </summary>
	void CreateParticlePipelineState();
	/// <summary>
	/// PostEffect 用のルートシグネチャを作成する。
	/// - フルスクリーンポスト処理用に SRV と CBV を持つレイアウトを定義して生成する。
	/// </summary>
	void CreatePostEffectRootSignature();
	/// <summary>
	/// PostEffect 用のグラフィックスパイプラインステートを作成する。
	/// - フルスクリーン描画に適した入力レイアウト（無し）／シェーダー／深度設定で PSO を生成する。
	/// </summary>
	void CreatePostEffectPipelineState();
	/// <summary>
	/// SkyBox 用のルートシグネチャを作成する。
	/// - SkyBox 描画に必要な SRV / CBV レイアウトを定義して生成する。
	/// </summary>
	void CreateSkyBoxRootSignature();
	/// <summary>
	/// SkyBox 用のグラフィックスパイプラインステートを作成する。
	/// - SkyBox 用シェーダーをコンパイルし、適切な深度／ラスタライザ設定で PSO を生成する。
	/// </summary>
	void CreateSkyBoxPipelineState();
public:
	/// <summary>Object3D 用のルートシグネチャを取得する。</summary>
	ID3D12RootSignature* GetRootSignature()const { return rootSignature.Get(); }
	/// <summary>Object3D 用のグラフィックスパイプラインステートを取得する。</summary>
	ID3D12PipelineState* GetGraphicsPipelineState()const { return graphicsPipelineState.Get(); }

	/// <summary>Sprite 用のルートシグネチャを取得する。</summary>
	ID3D12RootSignature* GetSpriteRootSignature()const { return spriteRootSignature.Get(); }
	/// <summary>Sprite 用のグラフィックスパイプラインステートを取得する。</summary>
	ID3D12PipelineState* GetSpriteGraphicsPipelineState()const { return spriteGraphicsPipelineState.Get(); }

	/// <summary>Particle 用のルートシグネチャを取得する。</summary>
	ID3D12RootSignature* GetParticleRootSignature()const { return particleRootSignature.Get(); }
	/// <summary>Particle 用のグラフィックスパイプラインステートを取得する。</summary>
	ID3D12PipelineState* GetParticleGraphicsPipelineState()const { return particleGraphicsPipelineState.Get(); }
	
    /// <summary>PostEffect 用のルートシグネチャを取得する。</summary>
	ID3D12RootSignature* GetPostEffectRootSignature()const { return postEffectRootSignature.Get(); }
	/// <summary>PostEffect 用のグラフィックスパイプラインステートを取得する。</summary>
	ID3D12PipelineState* GetPostEffectGraphicsPipelineState()const { return postEffectGraphicsPipelineState.Get(); }
	
	/// <summary>SkyBox 用のルートシグネチャを取得する。</summary>
	ID3D12RootSignature* GetSkyBoxRootSignature()const { return skyBoxRootSignature.Get(); }
	/// <summary>SkyBox 用のグラフィックスパイプラインステートを取得する。</summary>
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

