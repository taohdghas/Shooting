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
	//シングルトンインスタンスの取得
	static ModelManager* GetInstance();
	//初期化
	void Initialize(DirectXBase* directxBase);
	//終了
	void Finalize();
	//モデルファイルの読み込み
	void LoadModel(const std::string& filePath);
	//モデルの検索
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

