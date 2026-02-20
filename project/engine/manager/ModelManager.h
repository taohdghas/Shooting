#pragma once
#include "Model.h"
#include "ModelBase.h"
#include "DirectXBase.h"
#include <map>
#include <string>
#include <memory>

namespace MyEngine {

    // モデルマネージャー
    /// <summary>
    /// 3Dモデルデータの一元的な読み込み・管理・解放
    /// ファイルパスによるモデルの登録・検索・取得インターフェースの提供
    /// モデル共通リソース（ModelBase）の管理とDirectX基盤との連携
    /// シングルトンによるインスタンス管理
    /// </summary>
    class ModelManager {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// </summary>
        static ModelManager* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// </summary>
        void Initialize(DirectXBase* directx_base);

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// モデルファイルを読み込む。
        /// </summary>
        void LoadModel(const std::string& file_path);

        /// <summary>
        /// 登録済みのモデルを検索して返す。
        /// </summary>
        Model* FindModel(const std::string& file_path);
    public:
        // コンストラクタ
        ModelManager() {}
        // デストラクタ
        ~ModelManager() {}
    private:
        static std::unique_ptr<ModelManager> instance_;

        std::unique_ptr<ModelBase> model_base_;
        // コピーコンストラクタ
        ModelManager(const ModelManager&) = delete;
        // コピー代入演算子
        ModelManager& operator=(const ModelManager&) = delete;

        // モデルデータ
        std::map<std::string, std::unique_ptr<Model>> models_;
    };
}