#include "Player.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
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
	object_->SetLight(false);
	transform_.scale = { 0.25f,0.25f,0.25f };
	transform_.translate = { 0.0f,-1.5f,0.0f };
	//レティクル
	reticle_ = std::make_unique<Sprite>();
	reticle_->Initialize(SpriteBase::GetInstance(), "resources/black.png");
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
	//無敵タイマー更新
	if (invincibleTimer_ > 0.0f) {
		invincibleTimer_ -= DeltaTime;
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
	if (attackCooldown_ > 0.0f) return;

	Camera* camera = CameraManager::GetInstance()->GetActiveCamera();
	if (!camera) return;

	const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;

	//スクリーンからNDC
	float ndcX = (2.0f * reticleScreenPos_.x) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * reticleScreenPos_.y) / screenHeight;

	//ニアクリップ・ファークリップ
	Vector4 nearClip = { ndcX, ndcY, 0.0f, 1.0f };
	Vector4 farClip = { ndcX, ndcY, 1.0f, 1.0f };

	//ワールド座標変換
	Matrix4x4 invViewProj = Math::Inverse(camera->GetViewProjectionMatrix());
	Vector4 nearWorld = Math::Transform(nearClip, invViewProj);
	Vector4 farWorld = Math::Transform(farClip, invViewProj);

	//同次座標を正規化
	nearWorld.x /= nearWorld.w; nearWorld.y /= nearWorld.w; nearWorld.z /= nearWorld.w; nearWorld.w = 1.0f;
	farWorld.x /= farWorld.w;  farWorld.y /= farWorld.w;  farWorld.z /= farWorld.w;  farWorld.w = 1.0f;

	//カメラ位置
	Vector3 cameraPos = camera->GetTranslate();

	//カメラの逆行列からベクトルを取得
	Matrix4x4 invView = Math::Inverse(camera->GetViewMatrix());
	Vector3 cameraForward = Vector3(-invView.m[2][0], -invView.m[2][1], -invView.m[2][2]); // 前方向
	Vector3 cameraRight = Vector3(invView.m[0][0], invView.m[0][1], invView.m[0][2]);  // 右方向
	Vector3 cameraUp = Vector3(invView.m[1][0], invView.m[1][1], invView.m[1][2]);  // 上方向

	//オフセット距離
	float offsetForward = -1.0f; //前後
	float offsetRight = 0.0f; //左右
	float offsetUp = -0.5f; //上下

	//オフセットを適用
	Vector3 rayOrigin = cameraPos
		+ cameraForward * offsetForward
		+ cameraRight * offsetRight
		+ cameraUp * offsetUp;

	Vector3 rayDir = Math::Normalize(Vector3(farWorld.x, farWorld.y, farWorld.z) - rayOrigin);

	//弾速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity = rayDir * kBulletSpeed;

	auto newBullet = std::make_unique<playerBullet>();
	newBullet->Initialize(object3dBase_);
	newBullet->SetVelocity(velocity);
	newBullet->SetPosition(rayOrigin);
	bullets_.push_back(std::move(newBullet));

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
	const float moveSpeed = 10.0f;
	if (Input::GetInstance()->PushKey(DIK_LEFT))  reticleScreenPos_.x -= moveSpeed;
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) reticleScreenPos_.x += moveSpeed;
	if (Input::GetInstance()->PushKey(DIK_UP))    reticleScreenPos_.y -= moveSpeed;
	if (Input::GetInstance()->PushKey(DIK_DOWN))  reticleScreenPos_.y += moveSpeed;

	//画面端制限
	reticleScreenPos_.x = std::clamp(reticleScreenPos_.x, 0.0f, 1280.0f);
	reticleScreenPos_.y = std::clamp(reticleScreenPos_.y, 0.0f, 720.0f);

	reticle_->SetPosition(reticleScreenPos_);
	reticle_->Update();
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
//OBB取得
OBB Player::GetOBB()const {
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