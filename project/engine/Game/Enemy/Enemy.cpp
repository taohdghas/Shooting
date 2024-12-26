#include "Enemy.h"
#include "Math.h"

// 追尾をやめる距離
const float Enemy::kApproachDistance = 3.0f;

//初期化
void Enemy::Initialize(Object3dBase* object3dbase, Player* player) {
	object3dBase_ = object3dbase;
	player_ = player;
	//オブジェクト初期化
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("axis.obj");
	object_->SetScale({ 0.5f,0.5f,0.5f });
	transform_.translate = { 0.0f,0.0f,40.0f };
}
//更新
void Enemy::Update() {
	//フラグがたったらスキップ
	if (isDead_) {
		return;
	}
	if (isChasing_) {
		Vector3 playerPos = player_->GetPosition();
		Vector3 direction = Math::Subtract(playerPos, transform_.translate);
		float distance = Math::Length(direction);
		//一定距離未満なら追尾をやめる
		if (distance <= kApproachDistance) {
			isChasing_ = false;
			velocity_ = { 0.0f,0.0f,-0.2f };
		}
		else {
			direction = Math::Normalize(direction);
			velocity_ = Math::Scale(direction, 0.2f);
		}
	}
	//発射タイマーカウントダウン
	fireTimer--;
    //指定時間に達した
	if (fireTimer <= 0) {
		//弾を
	}

	//移動
	transform_.translate = Math::Add(transform_.translate, velocity_);

	//位置をobjectに反映
	object_->SetTranslate(transform_.translate);

	object_->Update();
}
//描画
void Enemy::Draw() {
	//フラグがたったらスキップ
	if (isDead_) {
		return;
	}

	object_->Draw();
}
//衝突時コールバック関数
void Enemy::OnCollision() {
	isDead_ = true;
}