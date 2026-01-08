#include "Player.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include "CameraManager.h"
#include "Platform.h"
#include <algorithm>

Player::Player() {}

Player::~Player() {}

// プレイヤーの初期化処理
void Player::Initialize(MyEngine::Object3dBase* object3d_base) {
	object3d_base_ = object3d_base;
	object_ = std::make_unique< MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("player/player.obj");
	object_->SetLight(false);
	transform_.scale = { 0.25f,0.25f,0.25f };
	transform_.translate = { 0.0f,-1.5f,0.0f };
	// レティクル（照準）の初期化
	reticle_ = std::make_unique< MyEngine::Sprite>();
	reticle_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/re.png");
	reticle_->SetSize({ 32,32 });
	reticle_->SetAnchorPoint({ 0.5f,0.5f });
}

// 毎フレームの更新処理
void Player::Update(bool is_start_animation_, bool is_returning_) {

	//攻撃クールタイム
	if (attack_cooldown_ > 0) {
		attack_cooldown_--;
	}
	// 回避クールタイム更新
	if (dodge_cooldown_ > 0) {
		dodge_cooldown_ -= kDeltaTime;
		if (dodge_cooldown_ < 0.0f) {
			dodge_cooldown_ = 0.0f;
		}
	}
	// 無敵タイマー更新
	if (invincible_timer_ > 0.0f) {
		invincible_timer_ -= kDeltaTime;
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](const std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	// スタート演出中は追従しない
	if (!is_start_animation_ && !is_returning_ && platform_) {
		// プラットフォーム追従処理
		if (is_follow_platform_) {
			//追従初回フレームで前フレーム位置を初期化
			if (!is_following_platform_initialized_) {
				prev_platform_pos_ = platform_->GetTranslate();
				is_following_platform_initialized_ = true;
			}

			// プラットフォームの移動量
			Vector3 platformDelta = platform_->GetTranslate() - prev_platform_pos_;
			prev_platform_pos_ = platform_->GetTranslate();

			//Z方向追従
			Vector3 pos = transform_.translate;
			pos.z += platformDelta.z;
			transform_.translate = pos;
		}
	} else {
		//スタート演出中初期化リセット
		is_following_platform_initialized_ = false;
	}

	//スタート演出中は不可
	if (!is_start_animation_ && !is_returning_) {
		// 移動処理
		Move();
		// ジャンプ処理
		Jump();
		// 二段ジャンプ時の継続回転
		if (jump_count_ == 2) {
			transform_.rotate.x += jump_rotate_speed_ * kDeltaTime;
		}

		// 攻撃入力判定
		if (MyEngine::Input::GetInstance()->IsMouseLeftPressed()) {
			Attack();
		}
		// 回避処理
		Dodge();
		// レティクルの座標・状態更新
		ReticleUpdate();
	}

	//ダメージ時色変化
	if (damage_color_timer_ > 0.0f) {
		damage_color_timer_ -= kDeltaTime;
		object_->SetColor({ 1.0f, 0.1961f, 0.0f, 1.0f });
	} else {
		object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	// Transform情報をObject3dへ反映
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

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
    // X方向のみ速度を初期化
    velocity_.x = 0.0f;

    // キー入力によるX方向の速度設定
    if (MyEngine::Input::GetInstance()->IsKeyPressed(DIK_A)) {
        velocity_.x = -0.1f;
    }
    if (MyEngine::Input::GetInstance()->IsKeyPressed(DIK_D)) {
        velocity_.x = 0.1f;
    }

    // 位置更新（
    transform_.translate.x += velocity_.x;

    // 移動範囲制限
    transform_.translate.x = std::clamp(transform_.translate.x, kGroundMinX, kGroundMaxX);
    if (transform_.translate.y < kGroundMinY) {
        transform_.translate.y = kGroundMinY;
    }
}

// ジャンプ処理
void Player::Jump() {
	// ジャンプ入力判定（最大回数未満ならジャンプ可能）
	if (MyEngine::Input::GetInstance()->IsKeyTriggered(DIK_W) && jump_count_ < kMaxJumpCount) {
		jump_velocity_ = jump_power_;
		jump_count_++;
	}

	// ジャンプ中の座標・速度更新
	if (jump_count_ > 0) {
		transform_.translate.y += jump_velocity_;
		jump_velocity_ += gravity_;

		// 着地判定
		if (transform_.translate.y <= ground_y_) {
			transform_.translate.y = ground_y_;
			jump_velocity_ = 0.0f;
			jump_count_ = 0;
			// 回転リセット
			transform_.rotate.x = 0.0f;
		}
	}
}

// 攻撃処理（弾発射）
void Player::Attack() {
	if (attack_cooldown_ > 0.0f) return;

	MyEngine::Camera* camera = MyEngine::CameraManager::GetInstance()->GetActiveCamera();
	if (!camera) return;

	const float screen_width = 1280.0f;
	const float screen_height = 720.0f;

	// スクリーン座標からNDC座標へ変換
	float ndc_x = (2.0f * reticle_screen_pos_.x) / screen_width - 1.0f;
	float ndc_y = 1.0f - (2.0f * reticle_screen_pos_.y) / screen_height;

	// ニア・ファークリップ座標
	Vector4 near_clip = { ndc_x, ndc_y, 0.0f, 1.0f };
	Vector4 far_clip = { ndc_x, ndc_y, 1.0f, 1.0f };

	// ワールド座標へ変換
	Matrix4x4 inv_view_proj = Math::Inverse(camera->GetViewProjectionMatrix());
	Vector4 near_world = Math::Transform(near_clip, inv_view_proj);
	Vector4 far_world = Math::Transform(far_clip, inv_view_proj);

	// 同次座標正規化
	near_world.x /= near_world.w; near_world.y /= near_world.w; near_world.z /= near_world.w; near_world.w = 1.0f;
	far_world.x /= far_world.w;  far_world.y /= far_world.w;  far_world.z /= far_world.w;  far_world.w = 1.0f;

	// カメラ位置取得
	Vector3 camera_pos = camera->GetTranslate();

	// カメラの逆行列から各軸ベクトル取得
	Matrix4x4 inv_view = Math::Inverse(camera->GetViewMatrix());
	Vector3 camera_forward = Vector3(-inv_view.m[2][0], -inv_view.m[2][1], -inv_view.m[2][2]); // 前方向
	Vector3 camera_right = Vector3(inv_view.m[0][0], inv_view.m[0][1], inv_view.m[0][2]);      // 右方向
	Vector3 camera_up = Vector3(inv_view.m[1][0], inv_view.m[1][1], inv_view.m[1][2]);         // 上方向

	// 弾発射位置のオフセット
	float offset_forward = -1.0f;
	float offset_right = 0.0f;
	float offset_up = -0.5f;

	Vector3 ray_origin = camera_pos
		+ camera_forward * offset_forward
		+ camera_right * offset_right
		+ camera_up * offset_up;

	// 弾の進行方向ベクトル
	Vector3 ray_dir = Math::Normalize(Vector3(far_world.x, far_world.y, far_world.z) - ray_origin);

	// 弾速度
	const float k_bullet_speed = 1.0f;
	Vector3 velocity = ray_dir * k_bullet_speed;

	// 弾生成・初期化・リスト追加
	auto new_bullet = std::make_unique<PlayerBullet>();
	new_bullet->Initialize(object3d_base_);
	new_bullet->SetVelocity(velocity);
	new_bullet->SetPosition(ray_origin);
	bullets_.push_back(std::move(new_bullet));

	// 攻撃クールタイムリセット
	attack_cooldown_ = attack_interval_;
}

// 回避処理
void Player::Dodge() {
	// 回避中の処理
	if (dodge_) {
		transform_.rotate.z += rotate_angle_ * kDeltaTime / apply_dodge_;
		dodge_timer_ += kDeltaTime;
		// 回避時間終了でフラグオフ・回転リセット
		if (dodge_timer_ > apply_dodge_) {
			dodge_ = false;
			dodge_timer_ = 0.0f;
			transform_.rotate.z = 0.0f;
		}
		return;
	}

	// クールダウン中は回避不可
	if (dodge_cooldown_ > 0.0f) {
		return;
	}

	// 回避入力判定
	if (MyEngine::Input::GetInstance()->IsKeyPressed(DIK_F)) {
		dodge_ = true;
		dodge_cooldown_ = dodge_interval_;
	}
}

// レティクル（照準）の座標更新
void Player::ReticleUpdate() {
	POINT mousePos = MyEngine::Input::GetInstance()->GetMousePosition();

	// マウス座標をVector2へ
	reticle_screen_pos_.x = static_cast<float>(mousePos.x);
	reticle_screen_pos_.y = static_cast<float>(mousePos.y);

	// 画面外防止
	reticle_screen_pos_.x = std::clamp(reticle_screen_pos_.x, 0.0f, 1280.0f);
	reticle_screen_pos_.y = std::clamp(reticle_screen_pos_.y, 0.0f, 720.0f);

	reticle_->SetPosition(reticle_screen_pos_);
	reticle_->Update();
}


// 衝突時コールバック（死亡フラグを立てる）
void Player::OnCollision() {
	is_dead_ = true;
}

// ダメージ処理
void Player::TakeDamage(int damage) {
	// 無敵時間中はダメージ無効
	if (invincible_timer_ > 0.0f) {
		return;
	}
#ifdef USE_IMGUI
	// デバッグ用無敵フラグがオンならダメージ無効
	if (is_invincible_) {
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
	invincible_timer_ = invincible_time_;
	damage_color_timer_ = damage_color_duration_;

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
			ImGui::Text("Dodge Timer: %.2f / %.2f", dodge_timer_, apply_dodge_);
			ImGui::Text("Dodge Cooldown: %.2f / %.2f", dodge_cooldown_, dodge_interval_);
		}

		if (ImGui::Button("無敵")) {
			is_invincible_ = !is_invincible_;
		}
		ImGui::TreePop();
	}
#endif
}

// OBB（当たり判定用の回転付きボックス）取得
OBB Player::GetOBB() const {
	OBB obb;
	obb.center = transform_.translate;

	Matrix4x4 rot_mat = Math::MakeRotateMatrix(transform_.rotate);
	obb.orientations[0] = Math::Normalize({ rot_mat.m[0][0], rot_mat.m[1][0], rot_mat.m[2][0] });
	obb.orientations[1] = Math::Normalize({ rot_mat.m[0][1], rot_mat.m[1][1], rot_mat.m[2][1] });
	obb.orientations[2] = Math::Normalize({ rot_mat.m[0][2], rot_mat.m[1][2], rot_mat.m[2][2] });

	obb.size.x = transform_.scale.x * dimensions_.x * 0.5f;
	obb.size.y = transform_.scale.y * dimensions_.y * 0.5f;
	obb.size.z = transform_.scale.z * dimensions_.z * 0.5f;

	return obb;
}
