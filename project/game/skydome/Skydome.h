#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"

//天球クラス
class Skydome
{
public:
	//初期化
	void Initialize(Object3dBase* object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
};

