#include "GameClearObject.h"
#include "ImGuiManager.h"

//初期化
void GameClearObject::Initialize() {

    //ゲームクリア文字モデル名
    std::string models[kNumLetters] = {
        "gameclearobject/g.obj",
        "gameclearobject/a.obj",
        "gameclearobject/m.obj",
        "gameclearobject/e.obj",
        "gameclearobject/c.obj",
        "gameclearobject/l.obj",
        "gameclearobject/e.obj",
        "gameclearobject/a.obj",
        "gameclearobject/r.obj"
    };

    //文字ごとの初期化
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].object = std::make_unique< MyEngine::Object3d>();
        letters_[i].object->Initialize(MyEngine::Object3dBase::GetInstance());
        letters_[i].object->SetModel(models[i]);

        letters_[i].transform.scale = { 0.6f,0.6f,0.6f };
        letters_[i].transform.rotate = { 0.0f,0.0f,0.0f };
        letters_[i].transform.translate = { kStartX + kSpacing * i, kBaseY, 1.0f };

        letters_[i].delay = i * 0.1f;
    }

	//push spaceスプライトの初期化
	push_space_sprite_ = std::make_unique< MyEngine::Sprite>();
	push_space_sprite_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/pushspace.png");
	push_space_sprite_->SetAnchorPoint({ 0.5f,0.5f });
	push_space_sprite_->SetSize({ 400.0f,100.0f });
	push_space_sprite_->SetPosition({ 640.0f, 480.0f + 100.0f });

}

//更新
void GameClearObject::Update()
{
    // 文字回転処理
    jump_timer_ += kDeltaTime;

    for (int i = 0; i < kNumLetters; i++) {

        if (i == current_index_) {
            float t = jump_timer_ / kJumpDuration;

            if (t < 1.0f) {
                // 回転
                float angle = t * 6.283185f;
                letters_[i].transform.rotate.y = angle;

                // ジャンプ
                letters_[i].transform.translate.y = kBaseY + sinf(t * kPi) * kJumpHeight;

                // 拡大
                float scaleFactor = 0.6f + sinf(t * 3.14159f) * 0.3f;
                letters_[i].transform.scale = { scaleFactor, scaleFactor, scaleFactor };

            } else {
                // 次の文字へ
                jump_timer_ = 0.0f;
                letters_[i].transform.rotate.y = 0.0f;
                letters_[i].transform.translate.y = kBaseY;
                letters_[i].transform.scale = { 0.6f,0.6f,0.6f };

                current_index_++;
                if (current_index_ >= kNumLetters) {
                    current_index_ = 0;
                }
            }
        } else {
            // 回転していない文字は初期位置
            letters_[i].transform.rotate.y = 0.0f;
            letters_[i].transform.translate.y = kBaseY;
            letters_[i].transform.scale = { 0.6f,0.6f,0.6f };
        }

        // 各文字に反映
        letters_[i].object->SetScale(letters_[i].transform.scale);
        letters_[i].object->SetRotate(letters_[i].transform.rotate);
        letters_[i].object->SetTranslate(letters_[i].transform.translate);
        letters_[i].object->Update();
    }

    // PushSpace の点滅
    alpha_timer_ += kDeltaTime;
    alpha_ = (sinf(alpha_timer_ * 3.0f) * 0.5f) + 0.5f;

    // スプライトのアルファ値を設定
    push_space_sprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
    push_space_sprite_->SetPosition({ 640.0f, 480.0f + 100.0f }); 
    push_space_sprite_->Update(); 
}

//描画
void GameClearObject::Draw() {
    //文字描画
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].object->Draw();
    }
}

//スプライト描画
void GameClearObject::DrawSprite() {
    //push spaceスプライト描画
    push_space_sprite_->Draw();
}

//デバッグ
void GameClearObject::Debug() {
#ifdef USE_IMGUI
    if (ImGui::Begin("GameClearObject SRT")) {
        ImGui::Text("GameClear Letters");
        ImGui::Separator();

        for (int i = 0; i < kNumLetters; i++) {
            std::string label = "Letter " + std::to_string(i);

            if (ImGui::TreeNode(label.c_str())) {
                ImGui::DragFloat3(("Scale##" + std::to_string(i)).c_str(), &letters_[i].transform.scale.x, 0.01f);
                ImGui::DragFloat3(("Rotate##" + std::to_string(i)).c_str(), &letters_[i].transform.rotate.x, 0.01f);
                ImGui::DragFloat3(("Translate##" + std::to_string(i)).c_str(), &letters_[i].transform.translate.x, 0.01f);
                ImGui::TreePop();
            }
        }

    }
    ImGui::End();
#endif
}