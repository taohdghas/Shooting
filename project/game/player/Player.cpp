#include "Player.h"
#include "Input.h"
#include "ImGuiManager.h"

Player::Player() {}

Player::~Player() {

}

//初期化
void Player::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("player/player.obj");
	object_->SetScale({ 0.1f,0.1f,0.1f });
	transform_.translate = { 0.0f,0.0f,0.0f };
}

//更新
void Player::Update() {
	//デスの場合スキップ
	if (isDead_) {
		return;
	}
	//攻撃クールタイム
	if (attackCooldown_ > 0) {
		attackCooldown_--;
	}
	//回避クールタイム
	if (dodgeCooldown_ > 0) {
		dodgeCooldown_ -= kDeltaTime;
		if (dodgeCooldown_ < 0.0f) {
			dodgeCooldown_ = 0.0f;
		}
	}
	//デスフラグが立った弾を削除
	bullets_.remove_if([](const std::unique_ptr<playerBullet>& bullet) {
		return bullet->IsDead();
		});
	//移動
	Move();
	//攻撃
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		Attack();
	}
	//3方向攻撃
	if (Input::GetInstance()->PushKey(DIK_R)) {
		ThreeAttack();
	}
	//回避
	Dodge();
	//回転と位置をobjectに反映
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

	object_->Update();

	//弾の更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}

}

//描画
void Player::Draw() {
	//デスの場合スキップ
	if (isDead_) {
		return;
	}
	//プレイヤーの描画
	object_->Draw();
	//プレイヤー弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
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
	//クールタイムが0より大きければスキップ
	if (attackCooldown_ > 0) {
		return;
	}
	//弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//弾を生成
	auto newBullet = std::make_unique<playerBullet>();
	newBullet->Initialize(object3dBase_);
	newBullet->SetVelocity(velocity);
	newBullet->SetPosition(transform_.translate);
	bullets_.push_back(std::move(newBullet));
	attackCooldown_ = attackInterval_;
}

//三方向攻撃
void Player::ThreeAttack() {
	//クールタイムが0より大きければスキップ
	if (attackCooldown_ > 0) {
		return;
	}

	//発射方向
	std::vector<Vector3> directions = {
		Vector3(-1, 0, 1),
		Vector3(0, 0, 1),
		Vector3(1, 0, 1)
	};

	for (const auto& dir : directions) {
		Vector3 velocity = Math::Normalize(dir);
		velocity = Math::Multiply(velocity, kBulletSpeed);

		auto newBullet = std::make_unique<playerBullet>();
		newBullet->Initialize(object3dBase_);
		newBullet->SetVelocity(velocity);
		newBullet->SetPosition(transform_.translate);
		bullets_.push_back(std::move(newBullet));
	}

	attackCooldown_ = attackInterval_;
}

//回避
void Player::Dodge() {

	//回避適用中
	if (dodge_) {
		//回転
		transform_.rotate.z += rotateAngle_ * kDeltaTime / applyDodge_;

		dodgeTimer_ += kDeltaTime;
		//回避適用時間を超えたらフラグをオフ
		if (dodgeTimer_ > applyDodge_) {
			dodge_ = false;
			dodgeTimer_ = 0.0f;
			transform_.rotate.z = 0.0f;
		}
		return;
	}

	//クールダウン中は処理をスキップ
	if (dodgeCooldown_ > 0.0f) {
		return;
	}

	//Fキーで回避発動
	if (Input::GetInstance()->PushKey(DIK_F)) {
		dodge_ = true;
		dodgeCooldown_ = dodgeInterval_;
	}
}

//衝突時コールバック
void Player::OnCollision() {
	isDead_ = true;
}

//HP減少関数
void Player::TakeDamage(int damage) {
	//回避フラグがオンならスキップ
	if (dodge_) {
		return;
	}
	//HPを減らす
	hp_ -= damage;
	//HPが0より少なくなったら衝突時コールバックを呼びだす。
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}

//デバック表示
void Player::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Player")) {
		Vector3 playerScale = object_->GetScale();
		Vector3 playerRotate = object_->GetRotate();
		Vector3 playerTranslate = object_->GetTranslate();
		ImGui::DragInt("PlayerHp", &hp_, 1);
		ImGui::DragFloat3("PlayerScale", &playerScale.x, 0.1f);
		ImGui::DragFloat3("PlayerRotate", &playerRotate.x, 0.1f);
		ImGui::DragFloat3("PlayerTranslate", &playerTranslate.x, 0.1f);
		object_->SetScale(playerScale);
		object_->SetRotate(playerRotate);
		object_->SetTranslate(playerTranslate);

		//回避
		if (ImGui::CollapsingHeader("Dodge Info")) {
			ImGui::Text("Is Dodging: %s", dodge_ ? "True" : "False");
			ImGui::Text("Dodge Timer: %.2f / %.2f", dodgeTimer_, applyDodge_);
			ImGui::Text("Dodge Cooldown: %.2f / %.2f", dodgeCooldown_, dodgeInterval_);
		}
		ImGui::TreePop();
	}
#endif
}