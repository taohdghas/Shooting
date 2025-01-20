#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <string>
#include <format>
#include <dxgidebug.h>
#include <dxcapi.h>
#include <vector>
#include <numbers>
#include <fstream>
#include <sstream>
#include <dinput.h>
#include "Sprite.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Transform.h"
#include "DirectionalLight.h"
#include "MaterialData.h"
#include "ModelData.h"
#include "Math.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ParticleEmitter.h"
#include "Framework.h"

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include "Logger.h"
using namespace Math;

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dinput8.lib")

class MyGame:public Framework
{
public:
	//初期化
	void Initialize()override;
    //終了
	void Finalize()override;
	//毎フレーム更新
	void Update()override;
	//描画
	void Draw()override;
private:
	//Sprite初期化
	std::vector<Sprite*>sprites;
	//3Dオブジェクト
	std::vector<Object3d*>object3ds;
	//パーティクルエミッター
	ParticleEmitter* particleEmitter = new ParticleEmitter();
};

