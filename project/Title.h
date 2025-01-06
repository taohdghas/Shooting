#pragma once
#include "DirectXBase.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "Transform.h"
class Title
{
public:
	//初期化
	void Initialize(Object3dBase* object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
private:
	DirectXBase* directxBase_;
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
};

