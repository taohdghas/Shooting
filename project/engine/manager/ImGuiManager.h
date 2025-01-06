#pragma once
#include "WindowsAPI.h"
#include "DirectXBase.h"
class ImGuiManager
{
public:
	//初期化
	void Initialize(WindowsAPI*windowsAPI,DirectXBase*directxBase);
private:
	DirectXBase* directbase_;
};

