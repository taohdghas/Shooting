#pragma once
#include "DirectXBase.h"
#include "PipeLineStateObject.h"
#include "Struct.h"

//SkyBoxのクラス
/// <summary>
/// スカイボックスの初期化・更新・描画処理の管理
/// 頂点・インデックス・マテリアル・変換行列等のDirectX 12リソース生成・管理
/// テクスチャファイルの読み込みとスカイボックスへの適用
/// ワールド座標変換やモデルデータの管理
/// 外部からのTransform取得インターフェースの提供
/// </summary>
class Skybox
{
public:
	/// <summary>
	/// 初期化
	/// <param name="texture_file_path">スカイボックスに適用するテクスチャファイルのパス</param>
	/// <returns>なし</returns>
	/// </summary>
	void Initialize(const std::string& texture_file_path);

	/// <summary>
	/// 更新
	/// <returns>なし</returns>
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// <returns>なし</returns>
	/// </summary>
	void Draw();

	/// <summary>
	/// インデックスデータを作成しインデックスバッファを生成する
	/// <returns>なし</returns>
	/// </summary>
	void CreateIndex();

	/// <summary>
	/// マテリアルデータ用のGPUバッファを作成して初期化
	/// <returns>なし</returns>
	/// </summary>
	void CreateMaterial();

	/// <summary>
	/// 座標変換用バッファを生成し初期化
	/// <returns>なし</returns>
	/// </summary>
	void CreateTransformation();

	/// <summary>
	/// 頂点データを生成して頂点バッファを作成
	/// <returns>なし</returns>
	/// </summary>
	void CreateVertex();

	/// <summary>
	/// 内部で保持している Transform を取得
	/// <returns>Transform構造体の参照</returns>
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