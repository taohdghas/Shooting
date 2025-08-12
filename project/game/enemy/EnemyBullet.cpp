#include "EnemyBullet.h"
#include "MyMath.h"

//初期化
void EnemyBullet::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemybullet.obj");
	object_->SetScale({ 1.0f,1.0f,1.0f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void EnemyBullet::Update() {

	transform_.translate = Math::Add(transform_.translate, velocity_);

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void EnemyBullet::Draw() {
	
	object_->Draw();
}
//衝突時コールバック
void EnemyBullet::OnCollision() {
	isDead_ = true;
}
//OBB取得関数
OBB EnemyBullet::GetOBB()const {
	OBB obb;
	//中心位置
	obb.center = transform_.translate;
	//回転行列
	Matrix4x4 rotMat = Math::MakeRotateMatrix(transform_.rotate);
	//各軸ベクトルを正規化
	obb.orientations[0] = Math::Normalize({ rotMat.m[0][0], rotMat.m[1][0], rotMat.m[2][0] }); // X軸
	obb.orientations[1] = Math::Normalize({ rotMat.m[0][1], rotMat.m[1][1], rotMat.m[2][1] }); // Y軸
	obb.orientations[2] = Math::Normalize({ rotMat.m[0][2], rotMat.m[1][2], rotMat.m[2][2] }); // Z軸

	obb.size = (transform_.scale * dimensions) * 0.5f;

	return obb;
}