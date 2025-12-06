#pragma once
#include "ParticleManager.h"
#include "Transform.h"

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
    /// - 指定した名前に対応するパーティクルグループの種類を ParticleManager から取得し、
    ///   種類に応じた既定値（count / frequency / transform）を設定する。
    /// - frequencyTime を 0 にリセットする。
    /// </summary>
    /// <param name="name">登録済みパーティクルグループの名前。</param>
    void Initialize(const std::string& name);

    /// <summary>
    /// 毎フレームの更新処理を行う。
    /// - 内部の frequencyTime を経過時間分だけ進め、frequency を超えていれば Emit() を呼ぶ。
    /// - 発生後は超過分を差し引いて frequencyTime を調整する。
    /// </summary>
    void Update();

    /// <summary>
    /// 即時にパーティクルを発生させる。
    /// - ParticleManager::Emit を呼び出して emitter_.count 分のパーティクルを生成する。
    /// </summary>
    void Emit();

public:
    // 座標をセット
    /// <summary>
    /// エミッターの発生位置を設定する。
    /// </summary>
    /// <param name="pos">設定するワールド座標。</param>
    void SetPosition(const Vector3& pos) { emitter_.transform.translate = pos; }

private:
    //ParticleTypeごとの初期値テーブル
    static const std::unordered_map<ParticleType, ParticleEmitterInitData> kEmitterInitTable;

    Emitter emitter_;
    std::string name_;
	const float kDeltaTime = 1.0f / 60.0f;
};

