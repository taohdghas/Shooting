#pragma once
#include "Model.h"
#include "ModelBase.h"
#include "DirectXBase.h"
#include <map>
#include <string>
#include <memory>

//テクスチャマネージャー
class ModelManager
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// - 初回呼び出し時に内部でインスタンスを生成して返す。
	/// </summary>
	static ModelManager* GetInstance();
	/// <summary>
	/// 初期化を行う。
	/// - 内部で ModelBase を生成し、引数の DirectXBase を渡して初期化する。
	/// </summary>
	/// <param name="directxBase">DirectX の共通処理を提供する <c>DirectXBase*</c></param>
	void Initialize(DirectXBase* directxBase);
	/// <summary>
	/// 終了処理を行う。
	/// - シングルトンインスタンスを削除して破棄する。
	/// </summary>
	void Finalize();
	/// <summary>
	/// モデルファイルを読み込む。
	/// - 既に読み込まれている場合は何もしない。
	/// - 新規なら <c>Model</c> を生成して初期化し、内部のマップに格納する。
	/// </summary>
	/// <param name="filePath">モデルファイル名または識別子（例: "model.obj" のファイル名）。</param>
	void LoadModel(const std::string& filePath);
	/// <summary>
	/// 登録済みのモデルを検索して返す。
	/// - 見つからなければ nullptr を返す。
	/// </summary>
	/// <param name="filePath">検索するモデルのファイル名／キー。</param>
	/// <returns>見つかれば <c>Model*</c>、なければ nullptr。</returns>
	Model* FindModel(const std::string& filePath);
private:
	static ModelManager* instance;
	ModelBase* modelBase = nullptr;
	// コンストラクタ
	ModelManager() {}
	// デストラクタ
	~ModelManager() {}
	// コピーコンストラクタ
	ModelManager(const ModelManager&) = delete;
	// コピー代入演算子
	ModelManager& operator=(const ModelManager&) = delete;
	//モデルデータ
	std::map<std::string, std::unique_ptr<Model>>models;
};

