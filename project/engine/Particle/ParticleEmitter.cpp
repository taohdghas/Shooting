#include "ParticleEmitter.h"

namespace MyEngine {

    // ParticleTypeごとの初期値テーブル定義
    const std::unordered_map<ParticleType, ParticleEmitter::ParticleEmitterInitData> ParticleEmitter::kEmitterInitTable = {
        { ParticleType::Normal,    { 1, 99.0f, {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} } },
        { ParticleType::Ring,      { 1, 99.0f, {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} } },
        { ParticleType::Cylinder,  { 1, 99.0f, {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} } },
        { ParticleType::Explosive, { 15, 5.0f, {1.0f,1.0f,1.0f}, {0.0f,0.0f,0.0f}, {0.0f,0.0f,0.0f} } }
    };

    // 初期化処理
    void ParticleEmitter::Initialize(const std::string& name) {
        name_ = name;
        emitter_.frequency_time_ = 0.0f; // 発生タイマーを初期化

        ParticleType type = ParticleManager::GetInstance()->GetParticleType(name_);

        auto it = kEmitterInitTable.find(type);
        assert(it != kEmitterInitTable.end());
        const auto& init = it->second;

        emitter_.count_ = init.count;
        emitter_.frequency_ = init.frequency;
        emitter_.transform.scale = init.scale;
        emitter_.transform.rotate = init.rotate;
        emitter_.transform.translate = init.translate;
    }

    // 更新処理（フレームごとに呼ばれる）
    void ParticleEmitter::Update() {
        emitter_.frequency_time_ += kDeltaTime; // 経過時間を加算

        // 発生頻度を超えた場合にパーティクルを発生
        if (emitter_.frequency_ <= emitter_.frequency_time_) {
            ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count_);

            // 余剰時間を次回の計算に引き継ぐ
            emitter_.frequency_time_ -= emitter_.frequency_;
        }
    }

    // 強制的にパーティクルを発生
    void ParticleEmitter::Emit() {
        ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count_);
    }
}