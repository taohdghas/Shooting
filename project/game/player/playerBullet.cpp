#include "playerBullet.h"
#include "MyMath.h"

//初期化
void playerBullet::Initialize(Object3dBase* object3dbase) {
	object3dbase_ = object3dbase;

	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dbase_);
	object_->SetModel("player/playerbullet.obj");
	object_->SetScale({ 1.0f,1.0f,1.0f });

	//デスタイマー
	deathTimer = kLifeTime;
}
//更新
void playerBullet::Update() {
	if (isDead_) {
		return;
	}
	//移動
	transform_.translate = Math::Add(transform_.translate, velocity_);
	//時間経過で消える
	if (--deathTimer <= 0) {
		isDead_ = true;
	}
	object_->SetTranslate(transform_.translate);
	object_->Update();
}
//描画
void playerBullet::Draw() {
	if (isDead_) {
		return;
	}
	object_->Draw();
}
//衝突時コールバック関数
void playerBullet::OnCollision() {
	isDead_ = true;
}
//OBB取得関数
OBB playerBullet::GetOBB()const {
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