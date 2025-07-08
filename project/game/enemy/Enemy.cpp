#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"

//初期化
void Enemy::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemy.obj");
	transform_.scale = { 0.5f,0.5f,0.5f };
	transform_.translate = { 0.0f,3.0f,20.0f };
}

//更新
void Enemy::Update() {

	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}
	
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		(*it)->Update();
		if ((*it)->IsDead()) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}

	//移動

	//攻撃(レーザー)
	Laser();

	//各Transformをobjectに適用
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
void Enemy::Draw() {
	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}

	object_->Draw();
	//弾の描画
	for (const auto& bullet : bullets_) {
		bullet->Draw();
	}
}

//攻撃(レーザー)
void Enemy::Laser() {
	//デスフラグオンならスルー
	if (isDead_) {
		return;
	}

	//発射タイマー

	static int fireTimer = 0;
	fireTimer++;

	//インターバルよりも小さい場合スルー
	if (fireTimer < kFireInterval) {
		return;
	}

	fireTimer = 0;

	if (!player_)return;

	Vector3 direction = Math::Subtract(player_->GetPosition(), transform_.translate);
	direction = Math::Normalize(direction);

	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3dBase_);
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}

//衝突時コールバック
void Enemy::onCollision() {
	isDead_ = true;
}

//HP減少関数
void Enemy::TakeDamage(int damage) {
	hp_ -= damage;
	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
		isDeathParticle_ = true;
	}
	//色変える
	damageColorTimer_ = DamageColorDuration;
}

//Debug
void Enemy::Debug() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("Enemy")) {
		ImGui::DragInt("EnemyHp", &hp_, 1);
		ImGui::DragFloat3("EnemyScale", &transform_.scale.x, 0.1f);
		ImGui::DragFloat3("EnemyRotate", &transform_.rotate.x, 0.1f);
		ImGui::DragFloat3("EnemyTranslate", &transform_.translate.x, 0.1f);

		ImGui::TreePop();
	}
#endif
}