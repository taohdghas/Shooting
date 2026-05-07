#pragma once
#include "ParticleManager.h"
#include "Transform.h"

namespace MyEngine {

	// パーティクルエミッタークラス
    /// <summary>
    /// パーティクル発生源の初期化・管理
    /// エミッターの位置・発生数・発生頻度などの状態管理
    /// 毎フレームのパーティクル発生制御（自動・即時発生）
    /// パーティクルマネージャとの連携によるパーティクル生成
    /// エミッター初期化データの管理と利用
    /// </summary>
	class ParticleEmitter
	{
	public:
		// エミッターの情報を保持
		/// <summary>
		/// エミッターの変換情報・発生数・発生頻度・時刻を保持する構造体
		/// </summary>
		struct Emitter {
			Transform transform;
			uint32_t count_;          // 発生数
			float frequency_;         // 発生頻度
			float frequency_time_;    // 頻度用時刻
		};

		// エミッター初期化データ
		/// <summary>
		/// エミッターの初期発生数・発生頻度・スケール・回転・位置をまとめた構造体
		/// </summary>
		struct ParticleEmitterInitData {
			int count;
			float frequency;
			Vector3 scale;
			Vector3 rotate;
			Vector3 translate;
		};

	public:
		/// <summary>
        /// エミッターを初期化
        /// <param name="name">エミッター名（std::string）</param>
        /// <returns>なし</returns>
        /// </summary>
		void Initialize(const std::string& name);

		/// <summary>
		/// 毎フレームの更新処理
		/// <returns>なし</returns>
		/// </summary>
		void Update();

		/// <summary>
		/// 即時にパーティクルを発生
		/// <returns>なし</returns>
		/// </summary>
		void Emit();

		/// <summary>
		/// エミッターの発生位置を設定
		/// <param name="pos">発生位置（Vector3）</param>
		/// <returns>なし</returns>
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