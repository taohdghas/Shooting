#pragma once
#include "Camera.h"

class CameraManager
{
public:
	//シングルトンインスタンス
	static CameraManager* GetInstance();
	//初期化
	void Initialize();
	//終了
	void Finalize();



private:
	static CameraManager* instance;
};

