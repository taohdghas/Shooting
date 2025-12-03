#include "ModelManager.h"

ModelManager* ModelManager::instance = nullptr;

// シングルトンインスタンス取得
ModelManager* ModelManager::GetInstance() {
    if (instance == nullptr) {
        instance = new ModelManager;
    }
    return instance;
}

// 初期化
void ModelManager::Initialize(DirectXBase* directxBase) {
    // ModelBase を生成・初期化
    modelBase = new ModelBase;
    modelBase->Initialize(directxBase);
}

// 終了
void ModelManager::Finalize() {
    delete instance;
    instance = nullptr;
}

// モデルファイルの読み込み
void ModelManager::LoadModel(const std::string& filePath) {
    // 既に読み込み済みなら処理しない
    if (models.contains(filePath)) {
        return;
    }

    // 新しいモデルを生成
    std::unique_ptr<Model> model = std::make_unique<Model>();

    // ファイルを読み込み初期化
    model->Initialize(modelBase, "resources", filePath);

    // map に格納
    models.insert(std::make_pair(filePath, std::move(model)));
}

// モデル検索
Model* ModelManager::FindModel(const std::string& filePath) {
    if (models.contains(filePath)) {
        return models.at(filePath).get();
    }
    return nullptr; // 一致するモデルなし
}
