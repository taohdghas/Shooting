#include "Player.h"
#include "Input.h"

//初期化
void Player::Initialize(Object3d* object) {
	object_ = object;
	object_->SetTranslate(Vector3{ 0.0f,0.0f,0.0f });
}

//更新
void Player::Update() {

	if (attackCooldown_ > 0) {
		attackCooldown_--;
	}

	//デスフラグが立った弾を削除
	bullets_.remove_if([](const std::unique_ptr<playerBullet>& bullet) {
		if (bullet->IsDead()) {
			return true;
		}
		return false;
		});

	//移動
	Move();

	//攻撃
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		Attack();
	}

	//弾の更新
	for (const std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	object_->SetTranslate(transform_.translate);

	object_->Update();
}

//描画
void Player::Draw() {

	//プレイヤー弾の描画
	for (const std::unique_ptr<playerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	object_->Draw();
}

//移動
void Player::Move() {
	if (Input::GetInstance()->PushKey(DIK_A)) {
		transform_.translate.x -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		transform_.translate.x += speed;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		transform_.translate.y -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_W)) {
		transform_.translate.y += speed;
	}
}

//攻撃
void Player::Attack() {

	if (attackCooldown_ > 0) {
		return;
	}

	//弾の生成(オブジェクト)
	std::unique_ptr<Object3d>bulletobject = std::make_unique<Object3d>();
	bulletobject->Initialize(Object3dBase::GetInstance());
	bulletobject->SetModel("plane.obj");
	bulletobject->SetScale(Vector3{ 0.1f,0.1f,0.1f });

	//弾インスタンス作成
	std::unique_ptr<playerBullet>bullet = std::make_unique<playerBullet>();
	bullet->Initialize(std::move(bulletobject));
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Vector3{ 0.0f,0.0f,1.0f });

	bullets_.push_back(std::move(bullet));

	//クールタイムに攻撃間隔を設定
	attackCooldown_ = attackInterval_;
}
