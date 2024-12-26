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
#include "Math.h"

#include "InputHandle.h"
#include "command.h"
#include "Player.h"
#include "Enemy.h"


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
//衝突判定と応答
void CheckAllCollisions(Player*player,Enemy*enemy) {
	//判定対象AとBの座標
	Vector3 posA, posB;

	//player弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player->GetBullets();

#pragma region プレイヤー弾と敵の当たり判定

#pragma endregion

#pragma region プレイヤーと敵の当たり判定
	//playerの座標
	posA = player->GetPosition();
	//enemyの座標
	posB = enemy->GetPosition();
	//衝突判定
	float length = Math::Length(posB - posA);
	float playerradius = player->GetRadius();
	float enemyradius = enemy->GetRadius();
	float radius = playerradius + enemyradius;
	if (length <= radius) {
		player->OnCollision();
		enemy->OnCollision();
	}
	
#pragma endregion

}
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
	//inputの初期化
	Input::GetInstance()->Initialize(windowsAPI);
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
	TextureManager::GetInstance()->Initialize(directxBase, srvManager);
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
	sprites[1]->Initialize(spriteBase, "resources/monsterBall.png");


	//3Dオブジェクト共通部
	Object3dBase* object3dBase = nullptr;
	//初期化
	object3dBase = new Object3dBase();
	object3dBase->Initialize(directxBase);
	//モデルマネージャ-
	ModelManager::GetInstance()->Initialize(directxBase);
	ModelManager::GetInstance()->LoadModel("plane.obj");
	ModelManager::GetInstance()->LoadModel("axis.obj");

    //カメラ
	Camera* camera = new Camera();
	camera->SetRotate({ 0.3f,0.0f,0.0f });
	camera->SetTranslate({ 0.0f,4.0f,-10.0f });

	//object3ds[0]->SetCamera(camera);
	//object3ds[1]->SetCamera(camera);

	Vector3 cameraRote = camera->GetRotate();
	Vector3 cameraPos = camera->GetTranslate();
	object3dBase->SetDefaultCamera(camera);

	//プレイヤー
	std::unique_ptr<Player>player_;
	player_ = std::make_unique<Player>();
	player_->Initialize(object3dBase);

	//敵
	std::unique_ptr<Enemy>enemy_;
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(object3dBase,player_.get());

	//コマンド
	Icommand* icommand_ = nullptr;

	//inputhandle
	InputHandle* inputHandle_ = nullptr;
	inputHandle_ = new InputHandle();
	inputHandle_->AssignMoveUpCommandPressKeyW();
	inputHandle_->AssignMoveUpCommandPressKeyS();
	inputHandle_->AssignMoveUpCommandPressKeyA();
	inputHandle_->AssignMoveUpCommandPressKeyD();
	inputHandle_->AssignAttackCommandPressKesSpace();
	//inputHandle_->AssignMoveDiagonalCommands();
#pragma endregion

#pragma region メインループ
	// ウィンドウのxボタンが押されるまでループ
	while (true) {

		// Windowにメッセージが来てたら最優先で処理させる
		if (windowsAPI->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}

		//入力の更新
		Input::GetInstance()->Update();

		//カメラの更新
		camera->Update();
		camera->SetMoveSpeed(0.01f);

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
		/*
		for (size_t i = 0; i < object3ds.size(); ++i) {
			Object3d* object3d = object3ds[i];
			object3d->Update();
		}
		*/

		std::vector<Icommand*> commands = inputHandle_->HandleInput();
		for (Icommand* command : commands) {
			if (command) {
				command->Exec(*player_);
			}
		}

		//プレイヤーの更新
		player_->Update();

		//敵の更新
		enemy_->Update();

		//衝突判定
		CheckAllCollisions(player_.get(), enemy_.get());
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

		//描画前処理
		directxBase->PreDraw();

		srvManager->PreDraw();

		//3Dオブジェクト描画準備
		object3dBase->DrawBaseSet();

		//共通描画設定
		spriteBase->DrawBaseSet();

		//プレイヤーの描画
		player_->Draw();

		//敵の描画
		enemy_->Draw();

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
	/*
	for (Object3d* object3d : object3ds) {
		delete object3d;
	}
	*/
	//3Dモデルマネージャの終了
	ModelManager::GetInstance()->Finalize();
	delete object3dBase;
	//テクスチャマネージャの終了
	TextureManager::GetInstance()->Finalize();
	delete srvManager;
	//DirectX解放
	delete directxBase;
	//入力解放
	Input::GetInstance()->Finalize();
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

