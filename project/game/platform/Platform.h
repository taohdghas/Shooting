#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Struct.h"

//地面のクラス
class Platform
{
public:
	//初期化
	void Initialize(Object3dBase*object3dbase);
	//更新
	void Update();
	//描画
	void Draw();
	//デバック
	void Debug();
private:
	Object3dBase* object3dBase_;
	Transform transform_;
	std::unique_ptr<Object3d>object_;
};

