#pragma once
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelManager.h"
#include "Transform.h"
#include "DirectXBase.h"

class skydome
{
public:
	void Initialize(Object3dBase* object3dbase);
	void Update();
	void Draw();
private:
	DirectXBase* directxBase_;
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
};

