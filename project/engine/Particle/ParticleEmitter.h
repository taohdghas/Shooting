#pragma once
#include "ParticleManager.h"
#include "Transform.h"

namespace MyEngine {

    // パーティクルエミッタークラス
    class ParticleEmitter {
    public:
        // エミッターの情報を保持
        struct Emitter {
            Transform transform;
            uint32_t count_;          // 発生数
            float frequency_;         // 発生頻度
            float frequency_time_;    // 頻度用時刻
        };

        // エミッター初期化データ
        struct ParticleEmitterInitData {
            int count;
            float frequency;
            Vector3 scale;
            Vector3 rotate;
            Vector3 translate;
        };


    public:
        /// <summary>
        /// エミッターを初期化する。
        /// </summary>
        void Initialize(const std::string& name);

        /// <summary>
        /// 毎フレームの更新処理を行う。
        /// </summary>
        void Update();

        /// <summary>
        /// 即時にパーティクルを発生させる。
        /// </summary>
        void Emit();

    public:
        /// <summary>
        /// エミッターの発生位置を設定する。
        /// </summary>
        void SetPosition(const Vector3& pos) { emitter_.transform.translate = pos; }

    private:
        //ParticleTypeごとの初期値テーブル
        static const std::unordered_map<ParticleType, ParticleEmitterInitData> kEmitterInitTable;

        Emitter emitter_;
        std::string name_;
        const float kDeltaTime = 1.0f / 60.0f;
    };
}