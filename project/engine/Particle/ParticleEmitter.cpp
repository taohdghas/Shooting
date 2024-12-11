#include "ParticleEmitter.h"

//初期化
void ParticleEmitter::Initialize() {
	emitter_.count = 3;
	emitter_.frequency = 0.5f;//0.5秒ごとに発生
	emitter_.frequencyTime = 0.0f;
	emitter_.transform.translate = { 0.0f,0.0f,0.0f };
	emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
	emitter_.transform.scale = { 1.0f,1.0f,1.0f };
}

//更新
void ParticleEmitter::Update() {
	//時刻を進める
	emitter_.frequencyTime += kDeltaTime;
	//発生頻度より大きければ発生
	if (emitter_.frequency < emitter_.frequencyTime) {
		ParticleManager::GetInstance()->Emit(name, emitter_.transform.translate, emitter_.count);
	}
}
//パーティクルの発生
void ParticleEmitter::Emit() {
	
}