#pragma once
#include "DirectXBase.h"

//3Dモデル共通部
class ModelBase
{
public:
	//初期化
	void Initialize(DirectXBase* directxBase);

	///getter///
	DirectXBase* GetDxBase()const { return directxBase_; }
private:
	DirectXBase* directxBase_;
};

