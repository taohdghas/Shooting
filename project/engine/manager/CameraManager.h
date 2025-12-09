#pragma once
#include "Camera.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace MyEngine {

    // カメラマネージャー
    class CameraManager {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// - 初回呼び出し時に内部でインスタンスを生成して返します。
        /// </summary>
        /// <returns>CameraManager の単一インスタンスへのポインタ。</returns>
        static CameraManager* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// - 内部のカメラ一覧をクリアし、アクティブカメラをリセットします。
        /// </summary>
        void Initialize();

        /// <summary>
        /// 終了処理を行う。
        /// - 内部のカメラ一覧をクリアし、シングルトンインスタンスを破棄します。
        /// </summary>
        void Finalize();

        /// <summary>
        /// カメラを追加する。
        /// - name が未登録かつ camera が nullptr でない場合に登録します。
        /// - まだアクティブカメラが設定されていなければ追加したカメラをアクティブにします。
        /// </summary>
        /// <param name="name">カメラ名（キー）。</param>
        /// <param name="camera">追加する Camera のポインタ。</param>
        void AddCamera(const std::string& name, Camera* camera);

        /// <summary>
        /// 現在のアクティブカメラを取得する。
        /// </summary>
        /// <returns>アクティブな Camera*（未設定なら nullptr）。</returns>
        Camera* GetActiveCamera();

        /// <summary>
        /// 名前からカメラを取得する。
        /// </summary>
        /// <param name="name">検索するカメラ名。</param>
        /// <returns>見つかれば Camera*、見つからなければ nullptr を返す。</returns>
        Camera* GetCamera(const std::string& name);

        /// <summary>
        /// 名前でアクティブカメラを設定する。
        /// - 指定名が登録済みであればそのカメラをアクティブにする。
        /// </summary>
        /// <param name="name">アクティブにするカメラの名前。</param>
        void SetActiveCamera(const std::string& name);

    public:
        CameraManager() = default;
        ~CameraManager() = default;

    private:
        static std::unique_ptr<CameraManager> instance_;
        //カメラ一覧
        std::unordered_map<std::string, Camera*> cameras_;
        //アクティブカメラ
        Camera* active_camera_ = nullptr;
    };
}