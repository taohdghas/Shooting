#include "Enemy.h"
#include "Player.h"
#include "ImGuiManager.h"
#include "MyMath.h"

// 初期化処理
void Enemy::Initialize(Object3dBase* object3dBase) {

    EnemyBase::Initialize(object3dBase);
    object_->SetModel("enemy/enemy.obj");
    object_->SetLight(false);

    // 初期Transform設定
    transform_.scale = { 0.5f, 0.5f, 0.5f };
    transform_.translate = { 0.0f, 3.0f, 20.0f };

    // 体力や状態リセット
    hp_ = 100;
    isDead_ = false;
    isDeathParticle_ = false;
    damageColorTimer_ = 0.0f;
}

// 更新処理
void Enemy::Update() {
    if (isDead_) {
        return;
    }

    // 弾の更新と削除
    for (auto it = bullets_.begin(); it != bullets_.end();) {
        (*it)->Update();
        if ((*it)->IsDead()) {
            it = bullets_.erase(it);
        } else {
            ++it;
        }
    }

    // 攻撃
    Laser();

    // TransformをObject3dへ反映
    object_->SetScale(transform_.scale);
    object_->SetRotate(transform_.rotate);
    object_->SetTranslate(transform_.translate);

    // ダメージ色タイマー処理
    if (damageColorTimer_ > 0.0f) {
        damageColorTimer_ -= DeltaTime_;
        object_->SetColor({ 0.8745f, 0.2274f, 0.2274f, 1.0f }); // ダメージ色
    } else {
        object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 通常色
    }

    object_->Update();
}

// 描画処理
void Enemy::Draw() {
    if (isDead_) {
        return;
    }

    object_->Draw();

    // 弾描画
    for (const auto& bullet : bullets_) {
        bullet->Draw();
    }
}

// レーザー攻撃（弾発射）
void Enemy::Laser() {
    if (isDead_) {
        return;
    }

    fireTimer_++;

    // 発射間隔未満なら発射しない
    if (fireTimer_ < kFireInterval) {
        return;
    }

    if (!player_) return;

    // プレイヤーとの距離計算
    Vector3 toPlayer = Math::Subtract(player_->GetTranslate(), transform_.translate);
    float distance = Math::Length(toPlayer);

    // 一定距離以上なら発射しない
    if (distance > fireDistance_) {
        return;
    }

    // Z距離制限
    if (transform_.translate.z < player_->GetTranslate().z + attackStopDistanceZ_) {
        return;
    }

    // 発射タイマーリセット
    fireTimer_ = 0;

    // プレイヤー方向の正規化ベクトル
    Vector3 direction = Math::Normalize(toPlayer);

    // 弾生成
    auto bullet = std::make_unique<EnemyBullet>();
    bullet->Initialize(object3dBase_);
    bullet->SetTranslate(transform_.translate);
    bullet->SetVelocity(Math::Multiply(direction, 0.2f));

    bullets_.emplace_back(std::move(bullet));
}

// 衝突処理
void Enemy::OnCollision() {
    isDead_ = true;
    isDeathParticle_ = true;
}

// ダメージ処理
void Enemy::TakeDamage(int damage) {
    hp_ -= damage;
    damageColorTimer_ = damageColorDuration_;
    if (hp_ <= 0) {
        hp_ = 0;
        OnCollision();
    }
}

// デバッグUI
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

// OBB取得
OBB Enemy::GetOBB() const {
    OBB obb;
    obb.center = transform_.translate;

    Matrix4x4 rotMat = Math::MakeRotateMatrix(transform_.rotate);
    obb.orientations[0] = Math::Normalize({ rotMat.m[0][0], rotMat.m[1][0], rotMat.m[2][0] });
    obb.orientations[1] = Math::Normalize({ rotMat.m[0][1], rotMat.m[1][1], rotMat.m[2][1] });
    obb.orientations[2] = Math::Normalize({ rotMat.m[0][2], rotMat.m[1][2], rotMat.m[2][2] });

    obb.size = (transform_.scale * dimensions_) * 0.5f;

    return obb;
}
