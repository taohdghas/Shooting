#pragma once
#include "Camera.h"
#include <string>
#include <memory>
#include <unordered_map>

//カメラマネージャー
class CameraManager
{
public:
	//シングルトンインスタンス
	static CameraManager* GetInstance();
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//カメラ追加
	void AddCamera(const std::string& name,Camera*camera);
public:
	///Getter///
	//アクティブカメラを取得
	Camera* GetActiveCamera();
	//名前からカメラを取得
	Camera* GetCamera(const std::string& name);

	///Setter///
	//アクティブカメラセット
	void SetActiveCamera(const std::string& name);
private:
	static CameraManager* instance;
	//カメラ一覧
	std::unordered_map<std::string,Camera*>cameras;
	//現在のアクティブカメラ
	Camera* activeCamera = nullptr;
};

