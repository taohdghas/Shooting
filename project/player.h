#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

class player
{
public:
	player();
	void Initialize(Object3dBase*object3dbase);
	void Update();
	void Draw();
public:
	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	void SetPosition(const Vector3& position) { transform_.translate = position; }
private:
	Object3dBase* object3dBase_;
	Transform transform_;
	std::unique_ptr<Object3d>object_;
};

