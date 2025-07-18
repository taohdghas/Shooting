#pragma once
#include "Struct.h"
#include <d3d12.h>
#include <wrl.h>

class WorldTransform
{
public:
	//初期化
	void Initialize();
	//行列計算
	void UpdateMatrix();
public:
	//ローカルスケール
	Vector3 scale_ = { 1,1,1 };
	//X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0,0,0 };
	//ローカル座標
	Vector3 translation_ = { 0,0,0 };
	//ローカル→ワールド変換行列
	Matrix4x4 matWorld_;
	//親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

private:

};

