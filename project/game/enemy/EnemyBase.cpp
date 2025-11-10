#include "EnemyBase.h"

void EnemyBase::Initialize(Object3dBase* object3dBase) {
	object3dBase_ = object3dBase;
	object_ = std::make_unique<Object3d>();
	object_->Initialize(object3dBase_);
}

void EnemyBase::Update() {
	object_->Update();
}

void EnemyBase::Draw() {
}
void EnemyBase::Debug(int id) {
}
void EnemyBase::OnCollision() {
}
void EnemyBase::TakeDamage(int damage) {
}
OBB EnemyBase::GetOBB() const {
	OBB obb;
	return obb;
}
