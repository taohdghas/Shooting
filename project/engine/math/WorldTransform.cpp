#include "WorldTransform.h"
#include "MyMath.h"

//初期化
void WorldTransform::Initialize() {

}
//行列計算
void WorldTransform::UpdateMatrix() {
	//スケール、回転、平行移動おｗ合成して行列を計算する
	matWorld_ = Math::MakeAffineMatrix(scale_, rotation_, translation_);
	//親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Math::Multiply(matWorld_, parent_->matWorld_);
	}
}