#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

//初期化
void Enemy::Initialize(Object3dBase*object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
	object_->SetModel("enemy/enemy.obj");
	object_->SetLight(false);
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

	if (!player_)return;

	Vector3 toPlayer = Math::Subtract(player_->GetPosition(), transform_.translate);
	float distance = Math::Length(toPlayer);

	//距離が定数の値以上なら撃たない
	if (distance > fireDistance) {
		return;
	}

	//プレイヤーのZ座標+定数の値以上なら撃たない
	if (transform_.translate.z < player_->GetPosition().z + attackStopDisntanceZ) {
		return;
	}

	//発射タイマー0に
	fireTimer = 0;

	Vector3 direction = Math::Normalize(toPlayer);

	auto bullet = std::make_unique<EnemyBullet>();
	bullet->Initialize(object3dBase_);
	bullet->SetPosition(transform_.translate);
	bullet->SetVelocity(Math::Multiply(direction, 0.2f));

	bullets_.emplace_back(std::move(bullet));
}
//衝突時コールバック
void Enemy::onCollision() {
	isDead_ = true;
	isDeathParticle_ = true;
}
//HP減少関数
void Enemy::TakeDamage(int damage) {
	hp_ -= damage;
	//色変える
	damageColorTimer_ = damageColorDuration;
	//object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f });
	if (hp_ <= 0) {
		hp_ = 0;
		onCollision();
	}
}
//Debug
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
//OBB取得関数
OBB Enemy::GetOBB()const {
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
