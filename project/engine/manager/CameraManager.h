#pragma once
#include "Camera.h"
#include <string>
#include <memory>
#include <unordered_map>

namespace MyEngine {

     // カメラマネージャー
     /// <summary>
     /// 複数カメラインスタンスの登録・管理
     /// カメラ名によるカメラの追加・取得・切り替え
     /// アクティブカメラの管理と取得インターフェースの提供
     /// シングルトンによるインスタンス管理
     /// カメラリソースの初期化・解放処理
     /// </summary>
    class CameraManager {
    public:
        /// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>CameraManagerのインスタンス（ポインタ）</returns>
        /// </summary>
        static CameraManager* GetInstance();

        /// <summary>
        /// 初期化
        /// <returns>なし</returns>
        /// </summary>
        void Initialize();

        /// <summary>
        /// 終了
        /// <returns>なし</returns>
        /// </summary>
        void Finalize();

        /// <summary>
        /// カメラを追加
        /// <param name="name">カメラ名（std::string）</param>
        /// <param name="camera">追加するCameraのポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void AddCamera(const std::string& name, Camera* camera);

        /// <summary>
        /// 現在のアクティブカメラを取得
        /// <returns>アクティブなCameraのポインタ</returns>
        /// </summary>
        Camera* GetActiveCamera();

        /// <summary>
        /// 名前からカメラを取得
        /// <param name="name">カメラ名（std::string）</param>
        /// <returns>Cameraのポインタ</returns>
        /// </summary>
        Camera* GetCamera(const std::string& name);

        /// <summary>
        /// 名前でアクティブカメラを設定
        /// <param name="name">カメラ名（std::string）</param>
        /// <returns>なし</returns>
        /// </summary>
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