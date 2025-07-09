#include "Player.h"
#include "Input.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include <algorithm>

Player::Player() {}

Player::~Player() {

}

//初期化
void Player::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("player/player.obj");
	transform_.scale = { 0.25f,0.25f,0.25f };
	transform_.translate = { 0.0f,-1.5f,0.0f };
	//レティクル
	reticle_ = std::make_unique<Sprite>();
	//reticle_->Initialize(SpriteBase::GetInstance(), "resources/white.png");
	reticle_->SetSize({ 16,16 });
	reticle_->SetAnchorPoint({ 0.5f,0.5f });
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
		dodgeCooldown_ -= DeltaTime;
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
	//ジャンプ
	Jump();
	//二段ジャンプ時継続回転
	if (jumpCount_ == 2) {
		transform_.rotate.x += jumpRotateSpeed_ * DeltaTime;
	}

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
	//各Transformをobjectに反映
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

	//色タイマー更新
	if (damageColorTimer_ > 0.0f) {
		damageColorTimer_ -= DeltaTime;
		object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f });
	} else {
		//元の色
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); 
	}

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
	/*
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
	*/
	Vector3 newPos = transform_.translate;

	//左右移動
	if (Input::GetInstance()->PushKey(DIK_A)) {
		newPos.x -= speed;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		newPos.x += speed;
	}

	//移動制限
	newPos.x = std::clamp(newPos.x, groundminX, groundmaxX);
	if (newPos.y < groundminY) {
		newPos.y = groundminY;
	}

	transform_.translate = newPos;
}

//ジャンプ
void Player::Jump() {
	if (Input::GetInstance()->TriggerKey(DIK_W) && jumpCount_ < maxJumpCount_) {
		jumpVelocity_ = jumpPower_;
		jumpCount_++;
	}

	//ジャンプ中
	if (jumpCount_ > 0) {
		transform_.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		//着地
		if (transform_.translate.y <= groundY_) {
			transform_.translate.y = groundY_;
			jumpVelocity_ = 0.0f;
			jumpCount_ = 0;

			//回転リセット
			transform_.rotate.x = 0.0f;
		}
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
		transform_.rotate.z += rotateAngle_ * DeltaTime / applyDodge_;

		dodgeTimer_ += DeltaTime;
		//回避時間を超えたらフラグをオフ
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

	//回避発動
	if (Input::GetInstance()->PushKey(DIK_F)) {
		dodge_ = true;
		dodgeCooldown_ = dodgeInterval_;
	}
}

//レティクル更新
void Player::ReticleUpdate(const Matrix4x4& view, const Matrix4x4& projection, int screenWidth, int screenHeight) {
	//プレイヤーの向いている方向
	Vector3 offset = { 0.0f,0.0f,1.0f };
	//プレイヤーの行列の回転を反映
	offset = Math::TransformNormal(offset, Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate));
	//ベクトルの長さを変える
	offset = Math::Normalize(offset) * 25.0f;
	//レティクルの座標を設定
	Vector3 reticleWorldPos = transform_.translate + offset;
	//ビュー,プロジェクション,ビューポート行列合成
	Matrix4x4 ViewPort = Math::MakeViewportMatrix(0, 0, float(WindowsAPI::GetInstance()->kClientWidth),
		float(WindowsAPI::GetInstance()->kClientHeight), 0, 1);
	Matrix4x4 VPV = Math::Multiply(Math::Multiply(view, projection), ViewPort);
	//スクリーン座標変換
	Vector3 screenPos = Math::Transform(reticleWorldPos, VPV);
	//座標に適用
	reticle_->SetPosition({ screenPos.x,screenPos.y });
}

//衝突時コールバック
void Player::OnCollision() {
	isDead_ = true;
}

//HP減少関数
void Player::TakeDamage(int damage) {
#ifdef USE_IMGUI
	//無敵の際はスキップ(デバックのみ)
	if (isInvincible) {
		return;
	}
#endif

	//回避フラグがオンならスキップ
	if (dodge_) {
		return;
	}
	//HPを減らす
	hp_ -= damage;
	//HPが0より少なくなったら衝突時コールバック
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
	//色変える
	damageColorTimer_ = DamageColorDuration;
}

//デバック表示
void Player::Debug() {
#ifdef USE_IMGUI
	//Player
	//if (ImGui::TreeNode("Player")) {
	if (ImGui::TreeNodeEx("Player", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragInt("PlayerHp", &hp_, 1);
		ImGui::DragFloat3("PlayerScale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("PlayerRotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("PlayerTranslate", &transform_.translate.x, 0.1f);

		//Playerの色変更
		if (ImGui::ColorEdit4("PlayerColor", &color_.x)) {
			object_->SetColor(color_);
		}

		//回避
		if (ImGui::CollapsingHeader("回避")) {
			ImGui::Text("Is Dodging: %s", dodge_ ? "True" : "False");
			ImGui::Text("Dodge Timer: %.2f / %.2f", dodgeTimer_, applyDodge_);
			ImGui::Text("Dodge Cooldown: %.2f / %.2f", dodgeCooldown_, dodgeInterval_);
		}

		if (ImGui::Button("無敵")) {
			isInvincible = !isInvincible;
		}
		ImGui::TreePop();
	}
#endif
}