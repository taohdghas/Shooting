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
		/// モデルを初期化する。
		/// </summary>
		void Initialize(ModelBase* model_base, const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// モデルを描画する。
		/// </summary>
		void Draw();

		/// <summary>
		/// マテリアル情報のコピーを取得する。
		/// </summary>
		Material GetMaterialCopy() const;

		/// <summary>
		/// モデルデータを取得する。
		/// </summary>
		const ModelData& GetModelData() const { return model_data_; }

		/// <summary>
		/// マテリアルの色を設定する。
		/// </summary>
		void SetColor(const Vector4& color) { material_data_->color = color; }

		/// <summary>
		/// ライティングの有効/無効を設定する。
		/// </summary>
		void SetEnableLighting(bool enable) { material_data_->enableLighting = enable; }

	private:
		/// <summary>
		/// モデルから取得したデータを基に頂点バッファ用のデータ構築を行う。
		/// </summary>
		void VertexDataCreate();

		/// <summary>
		/// マテリアル情報（テクスチャインデックスや色、ライティング設定など）を作成し,
		/// </summary>
		void MaterialCreate();

		/// <summary>
		/// .mtl（マテリアル定義）ファイルを読み取り、MaterialData を構築して返す。
		/// </summary>
		MaterialData LoadMaterialTemplateFile(const std::string& directory_path, const std::string& filename);

		/// <summary>
		/// .obj 等のモデルファイルを読み取り、ModelData を構築して返す。
		/// </summary>
		ModelData LoadModelFile(const std::string& directory_path, const std::string& filename);
		/// <summary>
		/// Assimp の aiNode 構造体を自前の Node 構造に変換する。
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
