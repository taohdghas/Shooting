#include "Player.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
#include <algorithm>

Player::Player() {}

Player::~Player() {}

// プレイヤーの初期化処理
void Player::Initialize(Object3dBase* object3dbase) {
	object3dBase_ = object3dbase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("player/player.obj");
	object_->SetLight(false);
	transform_.scale = { 0.25f,0.25f,0.25f };
	transform_.translate = { 0.0f,-1.5f,0.0f };
	// レティクル（照準）の初期化
	reticle_ = std::make_unique<Sprite>();
	reticle_->Initialize(SpriteBase::GetInstance(), "resources/re.png");
	reticle_->SetSize({ 32,32 });
	reticle_->SetAnchorPoint({ 0.5f,0.5f });
}

// 毎フレームの更新処理
void Player::Update() {
	//デスの場合スキップ

	//攻撃クールタイム
	if (attackCooldown_ > 0) {
		attackCooldown_--;
	}
	// 回避クールタイム更新
	if (dodgeCooldown_ > 0) {
		dodgeCooldown_ -= DeltaTime;
		if (dodgeCooldown_ < 0.0f) {
			dodgeCooldown_ = 0.0f;
		}
	}
	// 無敵タイマー更新
	if (invincibleTimer_ > 0.0f) {
		invincibleTimer_ -= DeltaTime;
	}
	
	// デスフラグが立った弾を削除
	bullets_.remove_if([](const std::unique_ptr<playerBullet>& bullet) {
		return bullet->IsDead();
	});

	// 移動処理
	Move();
	// ジャンプ処理
	Jump();
	// 二段ジャンプ時の継続回転
	if (jumpCount_ == 2) {
		transform_.rotate.x += jumpRotateSpeed_ * DeltaTime;
	}

	// 攻撃入力判定
	if (Input::GetInstance()->IsKeyPressed(DIK_SPACE)) {
		Attack();
	}
	// 回避処理
	Dodge();

	// Transform情報をObject3dへ反映
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

	// レティクルの座標・状態更新
	ReticleUpdate();

	object_->Update();

	// 弾の更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}
}

// プレイヤー・弾の描画処理
void Player::Draw() {
	//デスの場合スキップ

	//プレイヤーの描画
	object_->Draw();
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

// レティクル（照準）の描画
void Player::ReticleDraw() {
	reticle_->Draw();
}

// プレイヤーの移動処理
void Player::Move() {
	Vector3 newPos = transform_.translate;

	// 左右移動入力
	if (Input::GetInstance()->IsKeyPressed(DIK_A)) {
		newPos.x -= speed;
	}
	if (Input::GetInstance()->IsKeyPressed(DIK_D)) {
		newPos.x += speed;
	}

	// 移動範囲制限
	newPos.x = std::clamp(newPos.x, groundminX, groundmaxX);
	if (newPos.y < groundminY) {
		newPos.y = groundminY;
	}

	transform_.translate = newPos;
}

// ジャンプ処理
void Player::Jump() {
	// ジャンプ入力判定（最大回数未満ならジャンプ可能）
	if (Input::GetInstance()->IsKeyTriggered(DIK_W) && jumpCount_ < maxJumpCount_) {
		jumpVelocity_ = jumpPower_;
		jumpCount_++;
	}

	// ジャンプ中の座標・速度更新
	if (jumpCount_ > 0) {
		transform_.translate.y += jumpVelocity_;
		jumpVelocity_ += gravity_;

		// 着地判定
		if (transform_.translate.y <= groundY_) {
			transform_.translate.y = groundY_;
			jumpVelocity_ = 0.0f;
			jumpCount_ = 0;
			// 回転リセット
			transform_.rotate.x = 0.0f;
		}
	}
}

// 攻撃処理（弾発射）
void Player::Attack() {
	if (attackCooldown_ > 0.0f) return;

	Camera* camera = CameraManager::GetInstance()->GetActiveCamera();
	if (!camera) return;

	const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;

	// スクリーン座標からNDC座標へ変換
	float ndcX = (2.0f * reticleScreenPos_.x) / screenWidth - 1.0f;
	float ndcY = 1.0f - (2.0f * reticleScreenPos_.y) / screenHeight;

	// ニア・ファークリップ座標
	Vector4 nearClip = { ndcX, ndcY, 0.0f, 1.0f };
	Vector4 farClip = { ndcX, ndcY, 1.0f, 1.0f };

	// ワールド座標へ変換
	Matrix4x4 invViewProj = Math::Inverse(camera->GetViewProjectionMatrix());
	Vector4 nearWorld = Math::Transform(nearClip, invViewProj);
	Vector4 farWorld = Math::Transform(farClip, invViewProj);

	// 同次座標正規化
	nearWorld.x /= nearWorld.w; nearWorld.y /= nearWorld.w; nearWorld.z /= nearWorld.w; nearWorld.w = 1.0f;
	farWorld.x /= farWorld.w;  farWorld.y /= farWorld.w;  farWorld.z /= farWorld.w;  farWorld.w = 1.0f;

	// カメラ位置取得
	Vector3 cameraPos = camera->GetTranslate();

	// カメラの逆行列から各軸ベクトル取得
	Matrix4x4 invView = Math::Inverse(camera->GetViewMatrix());
	Vector3 cameraForward = Vector3(-invView.m[2][0], -invView.m[2][1], -invView.m[2][2]); // 前方向
	Vector3 cameraRight = Vector3(invView.m[0][0], invView.m[0][1], invView.m[0][2]);      // 右方向
	Vector3 cameraUp = Vector3(invView.m[1][0], invView.m[1][1], invView.m[1][2]);         // 上方向

	// 弾発射位置のオフセット
	float offsetForward = -1.0f;
	float offsetRight = 0.0f;
	float offsetUp = -0.5f;

	Vector3 rayOrigin = cameraPos
		+ cameraForward * offsetForward
		+ cameraRight * offsetRight
		+ cameraUp * offsetUp;

	// 弾の進行方向ベクトル
	Vector3 rayDir = Math::Normalize(Vector3(farWorld.x, farWorld.y, farWorld.z) - rayOrigin);

	// 弾速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity = rayDir * kBulletSpeed;

	// 弾生成・初期化・リスト追加
	auto newBullet = std::make_unique<playerBullet>();
	newBullet->Initialize(object3dBase_);
	newBullet->SetVelocity(velocity);
	newBullet->SetPosition(rayOrigin);
	bullets_.push_back(std::move(newBullet));

	// 攻撃クールタイムリセット
	attackCooldown_ = attackInterval_;
}

// 回避処理
void Player::Dodge() {
	// 回避中の処理
	if (dodge_) {
		transform_.rotate.z += rotateAngle_ * DeltaTime / applyDodge_;
		dodgeTimer_ += DeltaTime;
		// 回避時間終了でフラグオフ・回転リセット
		if (dodgeTimer_ > applyDodge_) {
			dodge_ = false;
			dodgeTimer_ = 0.0f;
			transform_.rotate.z = 0.0f;
		}
		return;
	}

	// クールダウン中は回避不可
	if (dodgeCooldown_ > 0.0f) {
		return;
	}

	// 回避入力判定
	if (Input::GetInstance()->IsKeyPressed(DIK_F)) {
		dodge_ = true;
		dodgeCooldown_ = dodgeInterval_;
	}
}

// レティクル（照準）の座標更新
void Player::ReticleUpdate() {
	const float moveSpeed = 10.0f;
	if (Input::GetInstance()->IsKeyPressed(DIK_LEFT))  reticleScreenPos_.x -= moveSpeed;
	if (Input::GetInstance()->IsKeyPressed(DIK_RIGHT)) reticleScreenPos_.x += moveSpeed;
	if (Input::GetInstance()->IsKeyPressed(DIK_UP))    reticleScreenPos_.y -= moveSpeed;
	if (Input::GetInstance()->IsKeyPressed(DIK_DOWN))  reticleScreenPos_.y += moveSpeed;

	// 画面端制限
	reticleScreenPos_.x = std::clamp(reticleScreenPos_.x, 0.0f, 1280.0f);
	reticleScreenPos_.y = std::clamp(reticleScreenPos_.y, 0.0f, 720.0f);

	reticle_->SetPosition(reticleScreenPos_);
	reticle_->Update();
}

// 衝突時コールバック（死亡フラグを立てる）
void Player::OnCollision() {
	isDead_ = true;
}

// ダメージ処理
void Player::TakeDamage(int damage) {
	// 無敵時間中はダメージ無効
	if (invincibleTimer_ > 0.0f) {
		return;
	}
#ifdef USE_IMGUI
	// デバッグ用無敵フラグがオンならダメージ無効
	if (isInvincible) {
		return;
	}
#endif
	// 回避中はダメージ無効
	if (dodge_) {
		return;
	}
	// HP減算
	hp_ -= damage;

	// 無敵時間・ダメージ色タイマーセット
	invincibleTimer_ = InvincivleTime;
	damageColorTimer_ = DamageColorDuration;

	// HPが0以下なら死亡処理
	if (hp_ <= 0) {
		hp_ = 0;
		OnCollision();
	}
}

// デバッグ表示（ImGuiによるパラメータ調整・状態表示）
void Player::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNodeEx("Player", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragInt("PlayerHp", &hp_, 1);
		ImGui::DragFloat3("PlayerScale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("PlayerRotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("PlayerTranslate", &transform_.translate.x, 0.1f);

		// プレイヤーの色変更
		if (ImGui::ColorEdit4("PlayerColor", &color_.x)) {
			object_->SetColor(color_);
		}

		// 回避状態表示
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

// OBB（当たり判定用の回転付きボックス）取得
OBB Player::GetOBB()const {
	OBB obb;
	obb.center = transform_.translate;

	// 回転行列から各軸ベクトルを算出・正規化
	Matrix4x4 rotMat = Math::MakeRotateMatrix(transform_.rotate);
	obb.orientations[0] = Math::Normalize({ rotMat.m[0][0], rotMat.m[1][0], rotMat.m[2][0] }); // X軸
	obb.orientations[1] = Math::Normalize({ rotMat.m[0][1], rotMat.m[1][1], rotMat.m[2][1] }); // Y軸
	obb.orientations[2] = Math::Normalize({ rotMat.m[0][2], rotMat.m[1][2], rotMat.m[2][2] }); // Z軸

	// スケールと寸法からサイズ算出
	obb.size = (transform_.scale * dimensions) * 0.5f;

	return obb;
}