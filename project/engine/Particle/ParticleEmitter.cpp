#include "ParticleEmitter.h"

//初期化
void ParticleEmitter::Initialize(std::string name) {
	name_ = name;
	emitter_.frequencyTime = 0.0f;

	ParticleType type = ParticleManager::GetInstance()->GetParticleType(name_);

	switch (type) {
	//通常
	case ParticleType::Normal:
		emitter_.count = 1;
		emitter_.frequency = 99.0f;
		emitter_.transform.scale = { 1.0f,1.0f,1.0f };
		emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
		emitter_.transform.translate = { 0.0f,0.0f,0.0f };
		break;
	//Ring型
	case ParticleType::Ring:
		emitter_.count = 1;
		emitter_.frequency = 99.0f;
		emitter_.transform.scale = { 1.0f,1.0f,1.0f };
		emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
		emitter_.transform.translate = { 0.0f,0.0f,0.0f };
		break;
	//Cylinder型
	case ParticleType::Cylinder:
		emitter_.count = 1;
		emitter_.frequency = 99.0f;
		emitter_.transform.scale = { 1.0f,1.0f,1.0f };
		emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
		emitter_.transform.translate = { 0.0f,0.0f,0.0f };
		break;
	//爆発
	case ParticleType::Explosive:
		emitter_.count = 15;
		emitter_.frequency = 5.0f;
		emitter_.transform.scale = { 1.0f,1.0f,1.0f };
		emitter_.transform.translate = { 0.0f,0.0f,0.0f };
		break;
	}
}

//更新
void ParticleEmitter::Update() {
	//時刻を進める
	emitter_.frequencyTime += kDeltaTime;
	//発生頻度より大きければ発生
	if (emitter_.frequency <= emitter_.frequencyTime) {
		ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count);
		//余計に過ぎた時間を加味して頻度計算
		emitter_.frequencyTime -= emitter_.frequency;
	}
}
//パーティクルの発生
void ParticleEmitter::Emit() {
	ParticleManager::GetInstance()->Emit(name_, emitter_.transform.translate, emitter_.count);
}