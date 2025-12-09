#include "GameOverObject.h"
#include "ImGuiManager.h"

// 初期化
void GameOverObject::Initialize() {
    // GAME OVER の１文字ずつのモデル名
    std::string models[kNumLetters] = {
        "gameoverobject/g.obj", "gameoverobject/a.obj", 
        "gameoverobject/m.obj", "gameoverobject/e.obj",
        "gameoverobject/o.obj", "gameoverobject/v.obj", 
        "gameoverobject/e.obj", "gameoverobject/r.obj"
    };

    // 各文字生成
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].obj = std::make_unique<Object3d>();
        letters_[i].obj->Initialize(Object3dBase::GetInstance());
        letters_[i].obj->SetModel(models[i]);

        letters_[i].transform.scale = { 1.0f, 1.0f, 1.0f };
        letters_[i].transform.rotate = { 0.0f, 0.0f, 0.0f };
        letters_[i].transform.translate = {
            startX_ + spacing_ * i,
            baseY_,
            1.0f
        };

        letters_[i].delay = i * 0.1f;
    }

    // retry オブジェクト
    retryObj_ = std::make_unique<Object3d>();
    retryObj_->Initialize(Object3dBase::GetInstance());
    retryObj_->SetModel("gameoverobject/retry.obj");

    retryTransform_.scale = { 0.7f, 0.7f, 0.7f };
    retryTransform_.rotate = { 0.0f, 0.0f, 0.0f };
    retryTransform_.translate = { -0.65f, -1.7f, 1.0f };
}

// 更新
void GameOverObject::Update() {
    // 文字ジャンプアニメーション
    jumpTimer_ += kDeltaTime;

    for (int i = 0; i < kNumLetters; i++) {
        float y = 0.0f;

        if (i == currentIndex_) {
            float t = jumpTimer_ / kJumpDuration;

            if (t < 1.0f) {
                y = sinf(t * 3.14159f) * kJumpHeight;
            } else {
                // 次の文字へ
                jumpTimer_ = 0.0f;
                currentIndex_++;
                if (currentIndex_ >= kNumLetters) {
                    currentIndex_ = 0;
                }
            }
        }

        letters_[i].transform.translate.y = y;
        letters_[i].obj->SetScale(letters_[i].transform.scale);
        letters_[i].obj->SetRotate(letters_[i].transform.rotate);
        letters_[i].obj->SetTranslate(letters_[i].transform.translate);
        letters_[i].obj->Update();
    }
    // retry 点滅
    alphaTimer_ += kDeltaTime;
    alpha_ = (sinf(alphaTimer_ * 3.0f) * 0.5f) + 0.5f;
    retryObj_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });

    retryObj_->SetScale(retryTransform_.scale);
    retryObj_->SetRotate(retryTransform_.rotate);
    retryObj_->SetTranslate(retryTransform_.translate);

    retryObj_->Update();
}

// 描画
void GameOverObject::Draw() {
    // １文字ずつ描画
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].obj->Draw();
    }
    retryObj_->Draw();
}

// デバッグ
void GameOverObject::Debug() {
#ifdef USE_IMGUI
    if (ImGui::Begin("GameOverObject SRT")) {
        // GameOver
        ImGui::Text("GameOver");
        ImGui::DragFloat3("GameOver Scale", &gameOverTransform_.scale.x, 0.01f);
        ImGui::DragFloat3("GameOver Rotate", &gameOverTransform_.rotate.x, 0.01f);
        ImGui::DragFloat3("GameOver Translate", &gameOverTransform_.translate.x, 0.01f);

        ImGui::Separator();

        // Retry
        ImGui::Text("Retry");
        ImGui::DragFloat3("Retry Scale", &retryTransform_.scale.x, 0.01f);
        ImGui::DragFloat3("Retry Rotate", &retryTransform_.rotate.x, 0.01f);
        ImGui::DragFloat3("Retry Translate", &retryTransform_.translate.x, 0.01f);
    }
    ImGui::End();
#endif
}
