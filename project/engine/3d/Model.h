#pragma once
#include "ModelBase.h"
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace MyEngine {

	// 3Dモデルクラス
	class Model {
	public:
		/// <summary>
		/// モデルを初期化する。
		/// 指定したディレクトリとファイル名からモデル（.obj 等）を読み込み、内部データと GPU リソースを作成する。
		/// </summary>
		/// <param name="model_base">描画共通設定を提供する ModelBase のポインタ。</param>
		/// <param name="directory_path">モデルファイルが存在するディレクトリのパス。</param>
		/// <param name="filename">読み込むモデルファイル名（例: "model.obj"）。</param>
		void Initialize(ModelBase* model_base, const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// モデルを描画する。
		/// 内部で作成した頂点・マテリアル等の GPU リソースを用いて描画コマンドを発行する。
		/// </summary>
		void Draw();

		/// <summary>
		/// モデルデータを取得する。
		/// ModelData の const 参照を返す（コピーは行わない）。
		/// </summary>
		/// <returns>内部に保持している ModelData の const 参照。</returns>
		const ModelData& GetModelData() const { return model_data_; }

		/// <summary>
		/// マテリアルの色を設定する。
		/// マテリアルのカラー情報を上書きすることで描画時の色を変更する。
		/// </summary>
		/// <param name="color">設定する RGBA カラー。</param>
		void SetColor(const Vector4& color) { material_data_->color = color; }

		/// <summary>
		/// ライティングの有効/無効を設定する。
		/// </summary>
		/// <param name="enable">true でライティングを有効、false で無効。</param>
		void SetEnableLighting(bool enable) { material_data_->enableLighting = enable; }

	private:
		/// <summary>
		/// モデルから取得したデータを基に頂点バッファ用のデータ構築を行う。
		/// GPU バッファの作成および vertex_data_ の初期化を行う想定。
		/// </summary>
		void VertexDataCreate();

		/// <summary>
		/// マテリアル情報（テクスチャインデックスや色、ライティング設定など）を作成し,
		/// GPU 用のマテリアルバッファを準備する。
		/// </summary>
		void MaterialCreate();

		/// <summary>
		/// .mtl（マテリアル定義）ファイルを読み取り、MaterialData を構築して返す。
		/// </summary>
		/// <param name="directory_path">.mtl ファイルが存在するディレクトリのパス。</param>
		/// <param name="filename">.mtl ファイル名。</param>
		/// <returns>読み取ったマテリアル情報を格納した MaterialData。</returns>
		MaterialData LoadMaterialTemplateFile(const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// .obj 等のモデルファイルを読み取り、ModelData を構築して返す。
		/// 頂点・法線・UV・マテリアル参照などを解析して model_data_ に設定する処理を含む。
		/// </summary>
		/// <param name="directory_path">モデルファイルが存在するディレクトリのパス。</param>
		/// <param name="filename">モデルファイル名（例: "model.obj"）。</param>
		/// <returns>読み取ったモデル情報を格納した ModelData。</returns>
		ModelData LoadModelFile(const std::string& directory_path, const std::string& filename);
		/// <summary>
		/// Assimp の aiNode 構造体を自前の Node 構造に変換する。
		/// 子ノードを再帰的に処理して階層構造を構築する。
		/// </summary>
		/// <param name="node">変換元の aiNode ポインタ。</param>
		/// <returns>変換後の Node（ローカル行列・名前・子ノードを含む）。</returns>
		Node ReadNode(aiNode* node);

	private:
		// ModelBaseのポインタ
		ModelBase* model_base_ = nullptr;
		// objファイルのデータ
		ModelData model_data_;
		// リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> material_resource_;
		// バッファリソース内のデータを指すポインタ
		VertexData* vertex_data_ = nullptr;
		Material* material_data_ = nullptr;

		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
	};
}
