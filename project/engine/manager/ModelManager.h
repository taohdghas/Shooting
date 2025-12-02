#pragma once
#include "Model.h"
#include "ModelBase.h"
#include "DirectXBase.h"
#include <map>
#include <string>
#include <memory>

// モデルマネージャー
class ModelManager {
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
    /// <param name="directx_base">DirectX の共通処理を提供する DirectXBase*</param>
    void Initialize(DirectXBase* directx_base);

    /// <summary>
    /// 終了処理を行う。
    /// - シングルトンインスタンスを削除して破棄する。
    /// </summary>
    void Finalize();

    /// <summary>
    /// モデルファイルを読み込む。
    /// - 既に読み込まれている場合は何もしない。
    /// - 新規なら Model を生成して初期化し、内部のマップに格納する。
    /// </summary>
    /// <param name="file_path">モデルファイル名または識別子（例: "model.obj" のファイル名）。</param>
    void LoadModel(const std::string& file_path);

    /// <summary>
    /// 登録済みのモデルを検索して返す。
    /// - 見つからなければ nullptr を返す。
    /// </summary>
    /// <param name="file_path">検索するモデルのファイル名／キー。</param>
    /// <returns>見つかれば Model*、なければ nullptr。</returns>
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
