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

#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"
#include "Logger.h"
using namespace Math;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
		sprite->SetPosition({ 100.0f * i,50.0f });
		sprites.push_back(sprite);
	}
	sprites[0]->Initialize(spriteBase, "resources/uvChecker.png");
	sprites[1]->SettextureSize({ 10.0f,10.0f });
	sprites[1]->Initialize(spriteBase,"resources/monsterBall.png");


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
	object3ds[0]->SetTranslate({-2.0f,0.0f,0.0f});
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

		//入力の更新
		input->Update();

		//カメラの更新
		camera->Update();

		//Spriteの更新
		for (size_t i = 0; i < sprites.size(); ++i) {
			Sprite* sprite = sprites[i];

			//回転テスト
			//float rotaion = sprite->GetRotation();
			//rotaion += 0.01f;
			//sprite->SetRotation(rotaion);

			//Spriteの更新
			sprite->Update();
		}

		//3Dオブジェクトの更新
		for (size_t i = 0; i < object3ds.size(); ++i) {
			Object3d* object3d = object3ds[i];
			object3d->Update();
		}
		/*

		//色変化テスト
		Vector4 color = sprite->GetColor();
		color.x += 0.01f;
		if (color.x > 1.0f) {
			color.x -= 1.0f;
		}
		sprite->SetColor(color);
		*/
		/*
		Vector2 size = sprite->GetSize();
		size.x += 0.1f;
		size.y += 0.1f;
		sprite->SetSize(size);
		*/
		
		/*
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Setting");
		ImGui::DragFloat3("CameraRotation", &cameraRote.x, 0.01f);
		ImGui::DragFloat3("CameraPosition", &cameraPos.x, 0.01f);
		camera->SetRotate(cameraRote);
		camera->SetTranslate(cameraPos);

		ImGui::End();
		ImGui::Render();

		ImGui::Begin("Set");
		ImGui::ColorEdit4("*materialData", &materialData->color.x);
		ImGui::Checkbox("useMonsterBall", &useMonsterBall);
		ImGui::ColorEdit4("*Light", &directionalLightData->color.x);
		ImGui::SliderFloat3("*LightDirection", &directionalLightData->direction.x, -2.0f, 2.0f);
		ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
		ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		ImGui::DragFloat3("CameraTranslation", &transform.rotate.x, 0.01f);
		ImGui::End();
		ImGui::Render();
		/*
		directionalLightData->direction = Normalize(directionalLightData->direction);

		//UVTransform用の行列
		Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSprite.scale);
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeRotateZMatrix(uvTransformSprite.rotate.z));
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSprite.translate));
		materialDataSprite->uvTransform = uvTransformMatrix;
		*/
		
		//描画前処理
		directxBase->PreDraw();

		srvManager->PreDraw();

		//3Dオブジェクト描画準備
		object3dBase->DrawBaseSet();

		//共通描画設定
		spriteBase->DrawBaseSet();

		//スプライト描画 \fd
		for (Sprite* sprite : sprites) {
			//sprite描画処理
			//sprite->Draw();
		}

		//3Dオブジェクト描画
		for (Object3d* object3d : object3ds) {
			//object3d->Draw();
		}
		
		// 実際のcommandListのImGuiの描画コマンドを積む
		//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), directxBase->Getcommandlist().Get());
		
		//描画後処理
		directxBase->PostDraw();
	}
#pragma endregion
	/*
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	*/
	//CloseHandle(fenceEvent);
	delete camera;

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
