#include "Player.h"
#include "Input.h"
#include "MyMath.h"
#include "TextureManager.h"
#include "CameraManager.h"
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
	reticle_->Initialize(SpriteBase::GetInstance(), "resources/re.png");
	reticle_->SetSize({ 40,40 });
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
	//無敵タイマー更新
	if (invincibleTimer_ > 0.0f) {
		invincibleTimer_ -= DeltaTime;
	}
	//色タイマー更新
		if (damageColorTimer_ > 0.0f) {
			damageColorTimer_ -= DeltaTime;
			object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f });
		} else {
			//元の色
			object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
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

	//レティクル更新
	ReticleUpdate();

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
//レティクル描画
void Player::ReticleDraw() {
	reticle_->Draw();
}
//移動
void Player::Move() {
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


	Matrix4x4 viewport = Math::MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 1.0f);

	//ViewProjection
	Matrix4x4 viewProj = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();

	//逆行列
	Matrix4x4 vpv = Math::Multiply(viewProj, viewport);
	Matrix4x4 invVpv = Math::Inverse(vpv);

	Vector3 screenVec = { reticlePos_.x, reticlePos_.y, 1.0f };

	//ワールド座標に変換
	Vector3 worldReticlePos = Math::Transform(screenVec, invVpv);
	Vector3 dir = Math::Subtract(worldReticlePos, transform_.translate);
	dir = Math::Normalize(dir);

	// 弾生成
	auto newBullet = std::make_unique<playerBullet>();
	newBullet->Initialize(object3dBase_);
	newBullet->SetPosition(transform_.translate);
	newBullet->SetVelocity(Math::Multiply(dir, kBulletSpeed));

	bullets_.push_back(std::move(newBullet));
	attackCooldown_ = attackInterval_;
	/*
	   if (attackCooldown_ > 0) { return; }

    Vector3 dir = Math::Normalize(worldReticlePos_ - transform_.translate);

    auto newBullet = std::make_unique<playerBullet>();
    newBullet->Initialize(object3dBase_);
    newBullet->SetPosition(transform_.translate);
    newBullet->SetVelocity(Math::Multiply(dir, kBulletSpeed));
    bullets_.push_back(std::move(newBullet));

    attackCooldown_ = attackInterval_;
	*/
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
void Player::ReticleUpdate() {
	Vector2 reticlePosition = reticle_->GetPosition();

	if (Input::GetInstance()->PushKey(DIK_UP)) {
	    reticlePos_.y -= reticleSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		reticlePos_.y += reticleSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		reticlePos_.x -= reticleSpeed;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		reticlePos_.x += reticleSpeed;
	}

	reticle_->SetPosition(reticlePos_);
	reticle_->Update();
	/*
    if (Input::GetInstance()->PushKey(DIK_UP)) {
        reticleInputOffset_.y -= reticleSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_DOWN)) {
        reticleInputOffset_.y += reticleSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_LEFT)) {
        reticleInputOffset_.x -= reticleSpeed;
    }
    if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
        reticleInputOffset_.x += reticleSpeed;
    }

    //3Dレティクル
    const float kDistancePlayerTo3DReticle = 25.0f;
    Vector3 offset = {0, 0, 1.0f}; 
    offset = Math::Normalize(offset) * kDistancePlayerTo3DReticle;
    worldReticlePos_ = transform_.translate + offset;

    Matrix4x4 viewProj = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
    Matrix4x4 viewport = Math::MakeViewportMatrix(0, 0, 1280, 720, 0.0f, 1.0f);
    Matrix4x4 matVPV = Math::Multiply(viewProj, viewport);
    Vector3 screenPos = Math::Transform(worldReticlePos_, matVPV);

    //オフセットを反映
    screenPos.x += reticleInputOffset_.x;
    screenPos.y += reticleInputOffset_.y;

    //2Dレティクル位置をセッ
    reticle_->SetPosition({screenPos.x, screenPos.y});
    reticle_->Update();

    //スクリーン位置を逆変換
    float ndcX = (screenPos.x / 1280.0f) * 2.0f - 1.0f;
    float ndcY = (screenPos.y / 720.0f) * -2.0f + 1.0f;
    Vector3 nearPoint = { ndcX, ndcY, 0.0f };
    Vector3 farPoint  = { ndcX, ndcY, 1.0f };

    Matrix4x4 invViewProj = Math::Inverse(viewProj);
    Vector3 worldNear = Math::Transform(nearPoint, invViewProj);
    Vector3 worldFar  = Math::Transform(farPoint,  invViewProj);
    worldReticlePos_ = worldNear + Math::Normalize(worldFar - worldNear) * kDistancePlayerTo3DReticle;
	*/
}


//衝突時コールバック
void Player::OnCollision() {
	isDead_ = true;
}

//HP減少関数
void Player::TakeDamage(int damage) {
	//無敵時間中
	if (invincibleTimer_ > 0.0f) {
		return;
	}
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

	//無敵時間をセット
	invincibleTimer_ = InvincivleTime;
	//色変える
	damageColorTimer_ = DamageColorDuration;

	//HPが0より少なくなったら衝突時コールバック
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
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