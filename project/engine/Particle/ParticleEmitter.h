#pragma once
#include "ParticleManager.h"
#include "Transform.h"

class ParticleEmitter
{
public:
	struct Emitter {
		Transform transform;
		uint32_t count;//発生数
		float frequency;//発生頻度
		float frequencyTime;//頻度用時刻
	};
public:
	void Initialize();
	void Update();
	//パーティクルの発生
	void Emit();
private:
	Emitter emitter_;
	const float kDeltaTime = 1.0f / 60.0f;
};

