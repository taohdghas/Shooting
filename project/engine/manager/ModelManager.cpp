#include "ModelManager.h"

std::unique_ptr<ModelManager> ModelManager::instance_ = nullptr;

// シングルトンインスタンス取得
ModelManager* ModelManager::GetInstance() {
    if (!instance_) {
        instance_ = std::make_unique<ModelManager>();
    }
    return instance_.get();
}

// 初期化
void ModelManager::Initialize(DirectXBase* directx_base) {
    // ModelBase を生成・初期化
    model_base_ = std::make_unique<ModelBase>();
    model_base_->Initialize(directx_base);
}

// 終了
void ModelManager::Finalize() {
    instance_.reset();
}

// モデルファイルの読み込み
void ModelManager::LoadModel(const std::string& file_path) {
    // 既に読み込み済みなら処理しない
    if (models_.contains(file_path)) {
        return;
    }

    // 新しいモデルを生成
    std::unique_ptr<Model> model = std::make_unique<Model>();

    // ファイルを読み込み初期化
    model->Initialize(model_base_.get(), "resources", file_path);

    // map に格納
    models_.insert(std::make_pair(file_path, std::move(model)));
}

// モデル検索
Model* ModelManager::FindModel(const std::string& file_path) {
    if (models_.contains(file_path)) {
        return models_.at(file_path).get();
    }
    return nullptr; 
}
