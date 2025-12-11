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
        /// </summary>
        static CameraManager* GetInstance();

        /// <summary>
        /// 初期化を行う。
        /// </summary>
        void Initialize();

        /// <summary>
        /// 終了処理を行う。
        /// </summary>
        void Finalize();

        /// <summary>
        /// カメラを追加する。
        /// </summary>
        void AddCamera(const std::string& name, Camera* camera);

        /// <summary>
        /// 現在のアクティブカメラを取得する。
        /// </summary>
        Camera* GetActiveCamera();

        /// <summary>
        /// 名前からカメラを取得する。
        /// </summary>
        Camera* GetCamera(const std::string& name);

        /// <summary>
        /// 名前でアクティブカメラを設定する。
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