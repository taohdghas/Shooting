#include "GameOverObject.h"
#include "Input.h"
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

    // 各文字生成・初期化
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].obj = std::make_unique< MyEngine::Object3d>();
        letters_[i].obj->Initialize(MyEngine::Object3dBase::GetInstance());
        letters_[i].obj->SetModel(models[i]);

        letters_[i].transform.scale = { 0.8f, 0.8f, 1.0f };
        letters_[i].transform.rotate = { 0.0f, 0.0f, 0.0f };
        letters_[i].transform.translate = {
            startX_ + spacing_ * i,
            baseY_,
            1.0f
        };
       
        letters_[i].delay = i * 0.1f;
    }

	//リトライスプライト生成・初期化
    retry_sprite_ = std::make_unique< MyEngine::Sprite>();
    retry_sprite_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/retry.png");
    retry_sprite_->SetAnchorPoint({ 0.5f, 0.5f });
    retry_sprite_->SetPosition({ 405, 580 });
    retry_sprite_->SetSize({ 300, 100 });
    //タイトルへ戻るスプライト生成・初期化
    back_title_sprite_ = std::make_unique< MyEngine::Sprite>();
    back_title_sprite_->Initialize(MyEngine::SpriteBase::GetInstance(), "resources/ui/back_title.png");
    back_title_sprite_->SetAnchorPoint({ 0.5f, 0.5f });
    back_title_sprite_->SetPosition({ 870, 580 });
	back_title_sprite_->SetSize({ 300, 100 });

    // 元サイズ保存
    retry_size_ = { 300,100 };
    back_title_size_ = { 300,100 };


}

// 更新
void GameOverObject::Update() {
    // 文字ジャンプアニメーション
    jumpTimer_ += kDeltaTime;
	// １文字ずつ処理
    for (int i = 0; i < kNumLetters; i++) {
        float y = 0.0f;
		// ジャンプ中の文字か判定
        if (i == currentIndex_) {
            float t = jumpTimer_ / kJumpDuration;
			// ジャンプ中
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
 
    //スプライト更新
    UpdateSprite();

}

// 描画
void GameOverObject::Draw() {
    // １文字ずつ描画
    for (int i = 0; i < kNumLetters; i++) {
        letters_[i].obj->Draw();
    }

}

// スプライト更新
void GameOverObject::UpdateSprite() {
    // 拡縮用タイマー
    scale_timer_ += kDeltaTime;

    // マウスが乗っている方を取得
    select_index_ = GetMouseHoverIndex();

    float scale = 1.0f + sinf(scale_timer_ * 5.0f) * 0.1f;

    //元サイズに戻す
    retry_sprite_->SetSize(retry_size_);
    back_title_sprite_->SetSize(back_title_size_);

	//選択されている方を拡大
    if (select_index_ == 0) {
        retry_sprite_->SetSize({ retry_size_.x * scale, retry_size_.y * scale });
    }

    if (select_index_ == 1) {
        back_title_sprite_->SetSize({ back_title_size_.x * scale, back_title_size_.y * scale });
    }

    auto input = MyEngine::Input::GetInstance();

    //クリック判定
    if (select_index_ != -1 && input->IsMouseLeftTriggered()) {
        if (select_index_ == 0) {
            menu_result_ = MenuResult::Retry;
        } else if (select_index_ == 1) {
            menu_result_ = MenuResult::BackTitle;
        }
    }

    // スプライト更新
    retry_sprite_->Update();
    back_title_sprite_->Update();
}

// スプライト描画
void GameOverObject::DrawSprite() {
    // スプライト描画
    retry_sprite_->Draw();
    back_title_sprite_->Draw();
}

// デバッグ
void GameOverObject::Debug() {
#ifdef USE_IMGUI
    if (ImGui::Begin("GameOverObject SRT")) {
        // 文字オブジェクトのSRT
        for (int i = 0; i < kNumLetters; i++) {
            std::string label = "Letter " + std::to_string(i);
            if (ImGui::TreeNode(label.c_str())) {
                ImGui::DragFloat3("Scale", &letters_[i].transform.scale.x, 0.01f, 0.01f, 10.0f);
                ImGui::DragFloat3("Rotate", &letters_[i].transform.rotate.x, 0.1f, -360.0f, 360.0f);
                ImGui::DragFloat3("Translate", &letters_[i].transform.translate.x, 0.1f, -1000.0f, 1000.0f);
                ImGui::TreePop();
            }
        }

        // リトライスプライト
        if (ImGui::TreeNode("Retry Sprite")) {
            Vector2 pos = retry_sprite_->GetPosition();
            Vector2 size = retry_sprite_->GetSize();
            if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f, 0.0f, 1280.0f)) {
                retry_sprite_->SetPosition(pos);
            }
            if (ImGui::DragFloat2("Size", (float*)&size, 1.0f, 1.0f, 1000.0f)) {
                retry_sprite_->SetSize(size);
            }
            ImGui::TreePop();
        }

        // タイトルへ戻るスプライト
        if (ImGui::TreeNode("Back Title Sprite")) {
            Vector2 pos = back_title_sprite_->GetPosition();
            Vector2 size = back_title_sprite_->GetSize();
            if (ImGui::DragFloat2("Position", (float*)&pos, 1.0f, 0.0f, 1280.0f)) {
                back_title_sprite_->SetPosition(pos);
            }
            if (ImGui::DragFloat2("Size", (float*)&size, 1.0f, 1.0f, 1000.0f)) {
                back_title_sprite_->SetSize(size);
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
#endif
}

int GameOverObject::GetMouseHoverIndex()
{
    if (IsMouseOnSprite(retry_sprite_.get())) return 0;
    if (IsMouseOnSprite(back_title_sprite_.get())) return 1;
    return -1;
}

bool GameOverObject::IsMouseOnSprite(MyEngine::Sprite* sprite)
{
    POINT mouse = MyEngine::Input::GetInstance()->GetMousePosition();

    Vector2 pos = sprite->GetPosition();
    Vector2 size = sprite->GetSize();
    Vector2 anchor = sprite->GetAnchorPoint();

    Vector2 leftTop;
    leftTop.x = pos.x - size.x * anchor.x;
    leftTop.y = pos.y - size.y * anchor.y;

    Vector2 rightBottom;
    rightBottom.x = leftTop.x + size.x;
    rightBottom.y = leftTop.y + size.y;

    if (mouse.x >= leftTop.x && mouse.x <= rightBottom.x &&
        mouse.y >= leftTop.y && mouse.y <= rightBottom.y) {
        return true;
    }

    return false;
}
