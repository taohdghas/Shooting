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
	/// - 指定したテクスチャファイルを読み込み、頂点・インデックス・マテリアル・変換行列用バッファを作成する。
	/// - 内部で PSO を初期化し、必要な GPU リソースを確保する想定。
	/// </summary>
	/// <param name="textureFilePath">使用するキューブマップまたはテクスチャファイルのパス。</param>
	void Initialize(std::string texture_file_path);
	/// <summary>
	/// 毎フレーム更新処理を行う。
	/// - Transform からワールド行列を計算し、カメラの ViewProjection 行列と組み合わせて WVP を更新する。
	/// - 更新結果は GPU 用の変換バッファに書き込まれる。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - 適切なルートシグネチャ／PSO をセットし、頂点バッファ／インデックスバッファ／マテリアル／テクスチャをバインドして描画コマンドを発行する。
	/// </summary>
	void Draw();
	/// <summary>
	/// インデックスデータを作成し、インデックスバッファを生成する。
	/// - インデックス配列のセットおよび GPU リソースへの転送を行う。
	/// </summary>
	void CreateIndex();
	/// <summary>
	/// マテリアルデータ用の GPU バッファを作成して初期化する。
	/// - マテリアルの色やライティングフラグ、UV トランスフォーム等を初期化して GPU バッファへ書き込む。
	/// </summary>
	void CreateMaterial();
	/// <summary>
	/// 座標変換（WVP/World）用バッファを生成し初期化する。
	/// - 描画時に CPU 側で更新した行列を GPU に反映するためのリソースを準備する。
	/// </summary>
	void CreateTransformation();
	/// <summary>
	/// 頂点データを生成して頂点バッファを作成する。
	/// - キューブの各面の頂点を作成し、GPU 用バッファにコピーする処理を行う。
	/// </summary>
	void CreateVertex();
public:
	/// <summary>
	/// 内部で保持している Transform を取得する（参照を返す）。
	/// - 取得した Transform を利用して外部からスケール/回転/平行移動を調整できる。
	/// </summary>
	/// <returns>内部 Transform の参照。</returns>
	Transform& GetTransform() { return transform_; }

private:
	DirectXBase* directx_base_ = nullptr;
	std::unique_ptr<PipelineStateObject> pso_;
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
