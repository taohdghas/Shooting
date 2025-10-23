#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化処理
void Enemy::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemy.obj");
	object_->SetLight(false);
	transform_.scale = { 0.5f,0.5f,0.5f };
	transform_.translate = { 0.0f,3.0f,20.0f };
}

// 毎フレームの更新処理
void Enemy::Update() {
	if (isDead_) {
		return;
	}

	// 弾の更新とデスフラグ判定（デッドならリストから削除）
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}

	// 攻撃処理（レーザー発射判定）
	Laser();

	// Transform情報をObject3dへ反映
	object_->SetScale(transform_.scale);
	object_->SetRotate(transform_.rotate);
	object_->SetTranslate(transform_.translate);

	// ダメージ時の色変更タイマー処理
	if (damageColorTimer_ > 0.0f) {
		damageColorTimer_ -= DeltaTime;
		object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f }); // ダメージ色
	} else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 通常色
	}

	object_->Update();

	// 弾の再更新
	for (const auto& bullet : bullets_) {
		bullet->Update();
	}
}

// 描画処理
void Enemy::Draw() {
	if (isDead_) {
		return;
	}

	object_->Draw();

	// 弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

// レーザー攻撃判定・発射処理
void Enemy::Laser() {
	if (isDead_) {
		return;
	}

	static int fireTimer = 0;
	fireTimer++;

	// 発射間隔未満なら発射しない
	if (fireTimer < kFireInterval) {
		return;
	}

	if (!player_) return;

	// プレイヤーとの距離計算
	Vector3 toPlayer = Math::Subtract(player_->GetTranslate(), transform_.translate);
	float distance = Math::Length(toPlayer);

	// 一定距離以上なら発射しない
	if (distance > fireDistance) {
		return;
	}

	// プレイヤーのZ座標＋定数より手前なら発射しない
	if (transform_.translate.z < player_->GetTranslate().z + attackStopDisntanceZ) {
		return;
	}

	// 発射タイマーリセット
	fireTimer = 0;

	// プレイヤー方向に正規化ベクトルを算出
	Vector3 direction = Math::Normalize(toPlayer);

	// 弾生成・初期化・速度設定
	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3dBase_);
	bullet->GetTranslate(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}

// 衝突時の処理（死亡フラグ・パーティクル発生）
void Enemy::onCollision() {
	isDead_ = true;
	isDeathParticle_ = true;
}

// ダメージ処理
void Enemy::TakeDamage(int damage) {
	hp_ -= damage;
	damageColorTimer_ = damageColorDuration; // ダメージ色タイマーセット
	if (hp_ <= 0) {
		hp_ = 0;
		onCollision(); // HP0で死亡処理
	}
}

// デバッグ用ImGui表示
void Enemy::Debug(int id) {
#ifdef USE_IMGUI
	std::string treeLabel = "Enemy##" + std::to_string(id);
	if (ImGui::TreeNode(treeLabel.c_str())) {
		std::string hpLabel = "EnemyHp##" + std::to_string(id);
		std::string scaleLabel = "EnemyScale##" + std::to_string(id);
		std::string rotateLabel = "EnemyRotate##" + std::to_string(id);
		std::string translateLabel = "EnemyTranslate##" + std::to_string(id);

		ImGui::DragInt(hpLabel.c_str(), &hp_, 1);
		ImGui::DragFloat3(scaleLabel.c_str(), &transform_.scale.x, 0.1f);
		ImGui::DragFloat3(rotateLabel.c_str(), &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3(translateLabel.c_str(), &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}

// OBB（当たり判定用の回転付きボックス）取得
OBB Enemy::GetOBB()const {
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
