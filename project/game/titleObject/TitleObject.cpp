#include "TitleObject.h"

//初期化
void TitleObject::Initialize() {
	//タイトルのオブジェクト
	title = std::make_unique<Object3d>();
	title->Initialize(Object3dBase::GetInstance());
	title->SetModel("title.obj");
	title->SetTranslate({ 0.0f,1.2f,1.0f });
	//pushspaceのオブジェクト
	pushspace = std::make_unique<Object3d>();
	pushspace->Initialize(Object3dBase::GetInstance());
	pushspace->SetModel("pushspace.obj");
	pushspace->SetTranslate({ 0.0f,-1.0f,1.0f });
	pushspace->SetScale({ 0.5f,0.5f,0.5f });
	//プレイヤーオブジェクト(外見のみ)
	playerobj = std::make_unique<Object3d>();
	playerobj->Initialize(Object3dBase::GetInstance());
	playerobj->SetModel("player/player.obj");
	playerobj->SetLight(true);
	playerobjTransform.scale = { 0.5f,0.5f,0.5f };
	playerobjTransform.rotate = { 0.0f,0.0f,0.0f };
	playerobjTransform.translate = { -1.5f,0.0f,0.0f };
}
//更新
void TitleObject::Update() {
	//タイトルオブジェクト
	title->Update();
	//pushSpaceのオブジェクト
	pushspace->Update();
	//プレイヤーオブジェクトの演出
	PlayerObjDirection();
}
//描画
void TitleObject::Draw() {
	//タイトルオブジェクト
	//title->Draw();
	//pushspaceオブジェクト
	//pushspace->Draw();
	//プレイヤーオブジェクト
	playerobj->Draw();
}
//プレイヤーオブジェクトの演出
void TitleObject::PlayerObjDirection() {
	//プレイヤーオブジェクトの回転
	playerobjTransform.rotate.y += RotateSpeed * kDeltaTime;

	playerobj->SetScale(playerobjTransform.scale);
	playerobj->SetRotate(playerobjTransform.rotate);
	playerobj->SetTranslate(playerobjTransform.translate);
	playerobj->Update();
}