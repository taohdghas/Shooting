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
	/// <summary>
	/// 3Dモデルデータの読み込み・管理
	/// モデルの初期化、描画処理の提供
	/// DirectX 12リソースの生成・管理
	/// マテリアルやライティング設定の制御
	/// Assimp等を用いた外部ファイルのパースとデータ変換
	/// </summary>
	class Model {
	public:
		/// <summary>
        /// モデルを初期化
        /// <param name="model_base">モデル共通部のポインタ</param>
        /// <param name="directory_path">モデルファイルのディレクトリパス</param>
        /// <param name="filename">モデルファイル名</param>
        /// <returns>なし</returns>
        /// </summary>
		void Initialize(ModelBase* model_base, const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// モデルを描画
		/// <returns>なし</returns>
		/// </summary>
		void Draw();

		/// <summary>
		/// マテリアル情報のコピーを取得
		/// <returns>マテリアル情報（Material）</returns>
		/// </summary>
		Material GetMaterialCopy() const;

		/// <summary>
		/// モデルデータを取得
		/// <returns>モデルデータ（const ModelData&）</returns>
		/// </summary>
		const ModelData& GetModelData() const { return model_data_; }

		/// <summary>
		/// マテリアルの色を設定
		/// <param name="color">色（Vector4）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetColor(const Vector4& color) { material_data_->color = color; }

		/// <summary>
		/// ライティングの有効/無効を設定
		/// <param name="enable">有効ならtrue、無効ならfalse</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetEnableLighting(bool enable) { material_data_->enableLighting = enable; }

	private:
		/// <summary>
		/// モデルから取得したデータを基に頂点バッファ用のデータ構築を行う
		/// <returns>なし</returns>
		/// </summary>
		void VertexDataCreate();

		/// <summary>
		/// マテリアル情報を作成
		/// <returns>なし</returns>
		/// </summary>
		void MaterialCreate();

		/// <summary>
		/// .mtlファイルを読み取り、MaterialData を構築して返す
		/// <param name="directory_path">ディレクトリパス</param>
		/// <param name="filename">ファイル名</param>
		/// <returns>MaterialData</returns>
		/// </summary>
		MaterialData LoadMaterialTemplateFile(const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// .obj 等のモデルファイルを読み取り、ModelData を構築して返す
		/// <param name="directory_path">ディレクトリパス</param>
		/// <param name="filename">ファイル名</param>
		/// <returns>ModelData</returns>
		/// </summary>
		ModelData LoadModelFile(const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// Assimp の aiNode 構造体を自前の Node 構造に変換
		/// <param name="node">aiNode構造体のポインタ</param>
		/// <returns>Node構造体</returns>
		/// </summary>
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
