#include "WorldTransform.h"
#include "MyMath.h"

// 初期化
void WorldTransform::Initialize() {
    // 現在は特に初期化処理はなし
}

// 行列計算
void WorldTransform::UpdateMatrix() {
    // ローカルスケール、ローカル回転、ローカル平行移動から
    // ローカル→ワールド変換行列を作成
    matWorld_ = Math::MakeAffineMatrix(scale_, rotation_, translation_);

    // 親トランスフォームがある場合は親のワールド行列を掛けて
    // ワールド座標に変換する
    if (parent_) {
        matWorld_ = Math::Multiply(matWorld_, parent_->matWorld_);
    }
}
