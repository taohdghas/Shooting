#pragma once
#include "DirectXBase.h"
#include "PipeLineStateObject.h"
#include "Struct.h"

//SkyBoxのクラス
class Skybox
{
public:
	/// <summary>
	/// 初期化を行う。
	/// </summary>
	void Initialize(const std::string& texture_file_path);
	/// <summary>
	/// 毎フレーム更新処理を行う。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// </summary>
	void Draw();
	/// <summary>
	/// インデックスデータを作成し、インデックスバッファを生成する
	/// </summary>
	void CreateIndex();
	/// <summary>
	/// マテリアルデータ用の GPU バッファを作成して初期化する。
	/// </summary>
	void CreateMaterial();
	/// <summary>
	/// 座標変換（WVP/World）用バッファを生成し初期化する。
	/// </summary>
	void CreateTransformation();
	/// <summary>
	/// 頂点データを生成して頂点バッファを作成する。
	/// </summary>
	void CreateVertex();
public:
	/// <summary>
	/// 内部で保持している Transform を取得する（参照を返す）。
	/// </summary>
	Transform& GetTransform() { return transform_; }

private:
	MyEngine::DirectXBase* directx_base_ = nullptr;
	std::unique_ptr<MyEngine::PipelineStateObject> pso_;
	VertexData* vertex_data_ = nullptr;
	Material* material_data_;
	Transform transform_;
	TransformationMatrix* transformation_matrix_data_;
	ModelData model_data_;
	std::string texture_file_path_;
	std::vector<VertexData> vertices_;
	uint32_t* index_data_;
	std::vector<uint32_t> indices_;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
	D3D12_INDEX_BUFFER_VIEW index_buffer_view_;
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> index_resource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> material_resource_;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformation_matrix_resource_;
};
