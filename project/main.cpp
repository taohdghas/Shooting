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

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	D3DResourceLeakChecker leakCheck;

	CoInitializeEx(0, COINIT_MULTITHREADED);

	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
#pragma region 基盤システム初期化
	//WindowsAPIポインタ
	WindowsAPI* windowsAPI = nullptr;
	//WindowsAPIの初期化
	windowsAPI = new WindowsAPI();
	windowsAPI->Initialize();
	//DirectXBaseポインタ
	DirectXBase* directxBase = nullptr;
	//DirectXの初期化
	directxBase = new DirectXBase();
	directxBase->Initialize(windowsAPI);
	//Inputポインタ
	Input* input = nullptr;
	//入力の初期化
	input = new Input();
	input->Initialize(windowsAPI);
	//SpriteBaseポインタ
	SpriteBase* spriteBase = nullptr;
	//SpriteBaseの初期化
	spriteBase = new SpriteBase;
	spriteBase->Initialize(directxBase);

	//SRVマネージャ
	SrvManager* srvManager = nullptr;
	srvManager = new SrvManager();
	srvManager->Initialize(directxBase);

	//テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(directxBase,srvManager);
	TextureManager::GetInstance()->LoadTexture("resources/uvChecker.png");
	TextureManager::GetInstance()->LoadTexture("resources/monsterBall.png");

	//Sprite初期化
	std::vector<Sprite*>sprites;
	for (uint32_t i = 0; i < 2; ++i) {
		Sprite* sprite = new Sprite();
		sprite->Initialize(spriteBase, "resources/uvChecker.png");
		sprite->SetPosition({ 100.0f,100.0f });
		sprites.push_back(sprite);
	}
	sprites[0]->Initialize(spriteBase, "resources/uvChecker.png");
	//sprites[1]->SettextureSize({ 10.0f,10.0f });
	//sprites[1]->Initialize(spriteBase,"resources/monsterBall.png");


	//3Dオブジェクト共通部
	Object3dBase* object3dBase = nullptr;
	//初期化
	object3dBase = new Object3dBase();
	object3dBase->Initialize(directxBase);
	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

	//3Dオブジェクト
	std::vector<Object3d*>object3ds;
	for (uint32_t i = 0; i < 2; ++i) {
		Object3d* object3d = new Object3d();
		object3d->Initialize(object3dBase);
		object3ds.push_back(object3d);
	}
	object3ds[0]->SetModel("plane.obj");
	object3ds[0]->SetTranslate({0.0f,0.0f,0.0f});
	object3ds[1]->SetModel("axis.obj");
	object3ds[1]->SetTranslate({ 2.0f,0.0f,0.0f });

	Vector3 objectrotate = object3ds[0]->GetRotate();

	//カメラ
	Camera* camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });
	object3ds[0]->SetCamera(camera);
	object3ds[1]->SetCamera(camera);
	
	Vector3 cameraRote = camera->GetRotate();
	Vector3 cameraPos = camera->GetTranslate();
	object3dBase->SetDefaultCamera(camera);

	//パーティクルマネージャ
	ParticleManager::GetInstance()->Initialize(directxBase, srvManager,camera);
	ParticleManager::GetInstance()->CreateparticleGroup("particle", "resources/circle.png");
	//パーティクルエミッター
	ParticleEmitter* particleEmitter = new ParticleEmitter();
	particleEmitter->Initialize("particle");
	particleEmitter->Emit();

	//ImGuiマネージャ
	ImGuiManager* imguimanager = new ImGuiManager();
	imguimanager->Initialize(windowsAPI,directxBase,srvManager);

	//サウンド
	Audio* audio_;
	audio_ = Audio::GetInstance();
	audio_->Initialize();
	SoundData soundData1 =  audio_->SoundLoadWave("resources/Alarm01.wav");

#pragma endregion


	//SRV切り替え
	bool useMonsterBall = true;

#pragma region メインループ
	// ウィンドウのxボタンが押されるまでループ
	while (true) {

		// Windowにメッセージが来てたら最優先で処理させる
		if (windowsAPI->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}
		
		//ImGui開始
		imguimanager->Begin();

		//入力の更新
		input->Update();

		//カメラの更新
		camera->Update();

		//Spriteの更新
		for (size_t i = 0; i < sprites.size(); ++i) {
			Sprite* sprite = sprites[i];

			Vector2 position = sprite->GetPosition();
			//Spriteの更新
			sprite->Update();
#ifdef USE_IMGUI
			ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
			ImGui::Begin(std::format("sprite##{}", i).c_str()); // 一意のIDを付与
			ImGui::DragFloat2(std::format("position##{}", i).c_str(), &position.x, 1.0f);
			ImGui::End();
#endif
			sprite->SetPosition(position);
		}

		//3Dオブジェクトの更新
		for (size_t i = 0; i < object3ds.size(); ++i) {
			Object3d* object3d = object3ds[i];
			object3d->Update();
		}
		
		//パーティクル更新
	//	particleEmitter->Update();
		//ParticleManager::GetInstance()->Update();

		//サウンド再生
		audio_->SoundPlayWave()

		//ImGui終了
		imguimanager->End();

		//描画前処理
		directxBase->PreDraw();

		srvManager->PreDraw();

		//3Dオブジェクト描画準備
		object3dBase->DrawBaseSet();

		//共通描画設定
		spriteBase->DrawBaseSet();

		//スプライト描画 
		for (Sprite* sprite : sprites) {
			//sprite描画処理
			sprite->Draw();
		}

		/*
		//3Dオブジェクト描画
		for (Object3d* object3d : object3ds) {
			object3d->Draw();
		}
		*/
		//パーティクル描画
		//ParticleManager::GetInstance()->Draw();
		
		// 実際のcommandListのImGuiの描画コマンドを積む
		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directxBase->Getcommandlist().Get());
		
		//ImGui描画
		imguimanager->Draw();

		//描画後処理
		directxBase->PostDraw();
	}
#pragma endregion
	
	//CloseHandle(fenceEvent);
	//Audio
	audio_->Finalize();
	//ImGui
	imguimanager->Finalize();
	//delete imguimanager;
	//パーティクルエミッター
	delete particleEmitter;
	//パーティクルマネージャーの終了
	ParticleManager::GetInstance()->Finalize();
	//カメラ
	delete camera;
	//オブジェクト
	for (Object3d* object3d : object3ds) {
		delete object3d;
	}
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	delete object3dBase;
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	delete srvManager;
	//DirectX解放
	delete directxBase;
	//入力解放
	delete input;
	//Sprite
	for (Sprite* sprite : sprites) {
		delete sprite;
	}
	//SpriteBase
	delete spriteBase;
	//WindowsAPIの終了処理
	windowsAPI->Finalize();
	//WindowsAPI関数
	delete windowsAPI;
	return 0;
}
