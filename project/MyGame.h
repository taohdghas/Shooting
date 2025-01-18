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
#include "Input.h"
#include <dinput.h>
#include "WindowsAPI.h"
#include "DirectXBase.h"
#include "D3DResourceLeakChecker.h"
#include "SpriteBase.h"
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
#include "TextureManager.h"
#include "Object3dBase.h"
#include "Object3d.h"
#include "ModelBase.h"
#include "Model.h"
#include "ModelManager.h"
#include "Camera.h"
#include "SrvManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "ImGuiManager.h"
#include "Audio.h"

#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include "Logger.h"
using namespace Math;
//
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"dinput8.lib")
class MyGame
{
public:
	//初期化
	void Initialize();
    //終了
	void Finalize();
	//毎フレーム更新
	void Update();
	//描画
	void Draw();
private:
	//WindowsAPIポインタ
	WindowsAPI* windowsAPI = nullptr;
	//DirectXBaseポインタ
	DirectXBase* directxBase = nullptr;
	//Inputポインタ
	Input* input = nullptr;
	//SpriteBaseポインタ
	SpriteBase* spriteBase = nullptr;
	//SRVマネージャ
	SrvManager* srvManager = nullptr;
	//Sprite初期化
	std::vector<Sprite*>sprites;
	//3Dオブジェクト共通部
	Object3dBase* object3dBase = nullptr;
	//3Dオブジェクト
	std::vector<Object3d*>object3ds;
	//カメラ
	Camera* camera = new Camera();
	//パーティクルエミッター
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	//ImGuiマネージャ
	ImGuiManager* imguimanager = new ImGuiManager();
	//サウンド
	Audio* audio_;
	SoundData soundData1 = audio_->SoundLoadWave("resources/Alarm01.wav");
};

