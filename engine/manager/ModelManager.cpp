#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

//シングルインスタンスの取得
ModelManager*ModelManager::GetInstance(){
	if (instance == nullptr) {
		instance = new ModelManager;
	}
	return instance;
}
//初期化
void ModelManager::Initialize(DirectXBase* directxBase) {
	modelBase = new ModelBase;
	modelBase->Initialize(directxBase);
}
//終了
void ModelManager::Finalize() {
	delete instance;
	instance = nullptr;
}
//モデルファイルの読み込み
void ModelManager::LoadModel(const std::string& filePath) {
	//読み込みモデルを検索
	if (models.contains(filePath)) {
		//読み込み済みなら早期return
		return;
	}
	//モデルの生成とファイル読み込み、初期化
	std::unique_ptr<Model>model = std::make_unique<Model>();
	model->Initialize(modelBase, "resources", filePath);
	//モデルをmapコンテナに格納する
	models.insert(std::make_pair(filePath, std::move(model)));
}
//モデルの検索
Model* ModelManager::FindModel(const std::string& filePath) {
	//読み込み済みモデルを検索
	if (models.contains(filePath)) {
		//読み込みモデルを戻り値としてreturn
		return models.at(filePath).get();
	}
	//ファイル名一致なし
	return nullptr;
}