#pragma once
#include "ModelBase.h"
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//3Dモデル
class Model
{
private:

public:
	/// <summary>
	/// モデルを初期化する。
	/// 指定したディレクトリとファイル名からモデル（.obj 等）を読み込み、内部データと GPU リソースを作成する。
	/// </summary>
	/// <param name="modelBase">描画共通設定を提供する ModelBase のポインタ。</param>
	/// <param name="directorypath">モデルファイルが存在するディレクトリのパス。</param>
	/// <param name="filename">読み込むモデルファイル名（例: "model.obj"）。</param>
	void Initialize(ModelBase* modelBase,const std::string&directorypath,const std::string&filename);
	/// <summary>
	/// モデルを描画する。
	/// 内部で作成した頂点・マテリアル等の GPU リソースを用いて描画コマンドを発行する。
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// モデルから取得したデータを基に頂点バッファ用のデータ構築を行う。
	/// GPU バッファの作成および vertexData の初期化を行う想定。
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
	/// <param name="directoryPath">.mtl ファイルが存在するディレクトリのパス。</param>
	/// <param name="filename">.mtl ファイル名。</param>
	/// <returns>読み取ったマテリアル情報を格納した MaterialData。</returns>
	 MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// .obj 等のモデルファイルを読み取り、ModelData を構築して返す。
	/// 頂点・法線・UV・マテリアル参照などを解析して modelData_ に設定する処理を含む。
	/// </summary>
	/// <param name="directoryPath">モデルファイルが存在するディレクトリのパス。</param>
	/// <param name="filename">モデルファイル名（例: "model.obj"）。</param>
	/// <returns>読み取ったモデル情報を格納した ModelData。</returns>
	 ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);
	/// <summary>
	/// Assimp の aiNode 構造体を自前の Node 構造に変換する。
	/// 子ノードを再帰的に処理して階層構造を構築する。
	/// </summary>
	/// <param name="node">変換元の aiNode ポインタ。</param>
	/// <returns>変換後の Node（ローカル行列・名前・子ノードを含む）。</returns>
	 Node ReadNode(aiNode* node);
public:
	/// <summary>
	/// モデルデータを取得する。
	/// ModelData の const 参照を返す（コピーは行わない）。
	/// </summary>
	/// <returns>内部に保持している ModelData の const 参照。</returns>
	const ModelData& GetModelData()const { return modelData_; }
	/// <summary>
	/// マテリアルの色を設定する。
	/// マテリアルのカラー情報を上書きすることで描画時の色を変更する。
	/// </summary>
	/// <param name="color">設定する RGBA カラー。</param>
	void SetColor(const Vector4& color) { materialData->color = color; }
	/// <summary>
	/// ライティングの有効/無効を設定する。
	/// </summary>
	/// <param name="enable">true でライティングを有効、false で無効。</param>
	void SetEnableLighting(bool enable) { materialData->enableLighting = enable; }//Lighting
private:
	//ModelBaseのポインタ
	ModelBase* modelBase_;
	//objファイルのデータ
	ModelData modelData_;
	//リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};

