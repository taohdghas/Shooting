#include "PlayerBullet.h"
#include "MyMath.h"
#include "ModelManager.h"
#include "Enemy.h"

// 弾の初期化処理
void PlayerBullet::Initialize(MyEngine::Object3dBase* object3d_base) {
	// モデルの読み込み
	MyEngine::ModelManager::GetInstance()->LoadModel("player/playerbullet.obj");
    // Object3dBaseの保存
	object3d_base_ = object3d_base;
	// 3Dオブジェクトの生成・初期化
	object_ = std::make_unique< MyEngine::Object3d>();
	object_->Initialize(object3d_base_);
	object_->SetModel("player/playerbullet.obj");
	object_->SetScale({ 1.5f, 1.5f, 1.5f });

	// 寿命タイマーの初期化
	death_timer_ = kLifeTime;
}

// 毎フレームの更新処理
void PlayerBullet::Update() {
    if (is_dead_) return;

    
     //ロックオン探索（未追尾時）
    if (!is_homing_ && is_homing_ready_) {

        const float lock_on_distance = 10.0f;
        const float lock_on_angle_deg = 7.0f;
        const float lock_on_cos =
            cosf(lock_on_angle_deg * 3.14159265f / 180.0f);

        Vector3 currentDir = Math::Normalize(velocity_);

        Enemy* bestTarget = nullptr;
        float bestDist = lock_on_distance;

        for (Enemy* enemy : enemies_) {
            if (!enemy || enemy->IsDead()) continue;

            Vector3 toEnemy = enemy->GetTranslate() - transform_.translate;
            float dist = Math::Length(toEnemy);
            if (dist > lock_on_distance) continue;

            Vector3 toEnemyDir = Math::Normalize(toEnemy);
            float dot = Math::Dot(currentDir, toEnemyDir);

            if (dot > lock_on_cos && dist < bestDist) {
                bestDist = dist;
                bestTarget = enemy;
            }
        }

        if (bestTarget) {
            SetHomingTarget(bestTarget);
        }
    }

    //ホーミング処理（安定版）
    if (is_homing_ && target_ && !target_->IsDead()) {

        Vector3 toTarget = target_->GetTranslate() - transform_.translate;
        float distance = Math::Length(toTarget);

        Vector3 desiredDir = Math::Normalize(toTarget);
        Vector3 currentDir = Math::Normalize(velocity_);

        float dot = Math::Dot(currentDir, desiredDir);

        //追尾解除条件
        //敵が後方に回った
        //敵が遠い
        if (dot < 0.0f || distance > 15.0f) {
            is_homing_ = false;
            target_ = nullptr;
        } else {
            //旋回制御
            const float turnRate = homing_strength_; 

            Vector3 newDir = Math::Normalize(
                currentDir * (1.0f - turnRate) +
                desiredDir * turnRate
            );

            velocity_ = newDir * speed_;
        }
    }

    // 移動
    transform_.translate += velocity_;

    // 寿命
    if (--death_timer_ <= 0) {
        is_dead_ = true;
    }

    object_->SetTranslate(transform_.translate);
    object_->Update();
}
// 弾の描画処理
void PlayerBullet::Draw() {
	if (is_dead_) {
		return;
	}
	object_->Draw();
}

// 衝突時の処理（消滅フラグを立てる）
void PlayerBullet::OnCollision() {
	is_dead_ = true;
}

// OBB（当たり判定用の回転付きボックス）取得
OBB PlayerBullet::GetOBB() const {
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