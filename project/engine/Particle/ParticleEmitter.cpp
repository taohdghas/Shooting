#include "ParticleEmitter.h"

// 初期化処理
void ParticleEmitter::Initialize(std::string name) {
    name_ = name;
    emitter_.frequencyTime = 0.0f; // 発生タイマーを初期化

    // ParticleManagerからパーティクルの種類を取得
    ParticleType type = ParticleManager::GetInstance()->GetParticleType(name_);

    // パーティクルタイプごとの初期設定
    switch (type) {
    case ParticleType::Normal: // 通常パーティクル
    case ParticleType::Ring:   // リング型
    case ParticleType::Cylinder: // シリンダー型
        emitter_.count = 1;
        emitter_.frequency = 99.0f; // 発生頻度（秒）
        emitter_.transform.scale = { 1.0f, 1.0f, 1.0f };
        emitter_.transform.rotate = { 0.0f, 0.0f, 0.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f };
        break;

    case ParticleType::Explosive: // 爆発型パーティクル
        emitter_.count = 15;        // 多数のパーティクルを一度に発生
        emitter_.frequency = 5.0f;  // 発生頻度
        emitter_.transform.scale = { 1.0f, 1.0f, 1.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f };
        break;
    case ParticleType::Smoke:
        emitter_.count = 2;            // 1度に3つ出す
        emitter_.frequency = 0.03f;    // 0.03秒ごと
        emitter_.transform.scale = { 1.0f, 1.0f, 1.0f };
        emitter_.transform.rotate = { 0.0f, 0.0f, 0.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f };
        break;
    case ParticleType::PlayerMove:
        emitter_.count = 1;              // 1つずつ出す
        emitter_.frequency = 0.01f;      // 高頻度
        emitter_.transform.scale = { 0.3f, 0.3f, 0.3f };
        emitter_.transform.rotate = { 0.0f, 0.0f, 0.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f };
        break;
    case ParticleType::EnemyDamage:
        emitter_.count = 10;               
        emitter_.frequency = 9.0f;     
        emitter_.transform.scale = { 1.4f, 1.4f, 1.4f };
        emitter_.transform.rotate = { 0.0f, 0.0f, 0.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f };
        break;
    case ParticleType::Confetti:
        emitter_.count = 15;             
        emitter_.frequency = 0.01f;     
        emitter_.transform.scale = { 0.2f, 0.2f, 0.2f };
        emitter_.transform.rotate = { 0.0f, 0.0f, 0.0f };
        emitter_.transform.translate = { 0.0f, 0.0f, 0.0f }; 
        break;

    }
}

// 更新処理（フレームごとに呼ばれる）
void ParticleEmitter::Update() {
    emitter_.frequencyTime += kDeltaTime; // 経過時間を加算

    // 発生頻度を超えた場合にパーティクルを発生
    if (emitter_.frequency <= emitter_.frequencyTime) {
        ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count);

        // 余剰時間を次回の計算に引き継ぐ
        emitter_.frequencyTime -= emitter_.frequency;
    }
}

// 強制的にパーティクルを発生
void ParticleEmitter::Emit() {
    ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count);
}
