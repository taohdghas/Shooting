#include "GameClearObject.h"
#include "ImGuiManager.h"

//初期化
void GameClearObject::Initialize() {

    //ゲームクリア文字モデル名
    std::string models[NumLetters] = {
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
    for (int i = 0; i < NumLetters; i++) {
        letters[i].obj = std::make_unique<Object3d>();
        letters[i].obj->Initialize(Object3dBase::GetInstance());
        letters[i].obj->SetModel(models[i]);

        letters[i].transform.scale = { 0.6f,0.6f,0.6f };
        letters[i].transform.rotate = { 0.0f,0.0f,0.0f };
        letters[i].transform.translate = { startX + spacing * i, baseY, 1.0f };

        letters[i].delay = i * 0.1f;
    }
    
    //PushSpaceオブジェクトの初期化
    pushspace = std::make_unique<Object3d>();
    pushspace->Initialize(Object3dBase::GetInstance());
    pushspace->SetModel("gameclearobject/pushspace.obj");
    pushspaceTransform.scale = { 0.5f,0.5f,0.5f };
    pushspaceTransform.rotate = { 0.0f,0.0f,0.0f };
    pushspaceTransform.translate = { -2.0f,-1.7f,1.0f };
    
}

//更新
void GameClearObject::Update()
{
    // 文字回転処理
    jumpTimer_ += DeltaTime;

    for (int i = 0; i < NumLetters; i++) {

        if (i == currentIndex_) {
            float t = jumpTimer_ / jumpDuration_;

            if (t < 1.0f) {
                // 回転
                float angle = t * 6.283185f;
                letters[i].transform.rotate.y = angle;

                // ジャンプ
                letters[i].transform.translate.y = baseY + sinf(t * 3.14159f) * jumpHeight_;

                // 拡大
                float scaleFactor = 0.6f + sinf(t * 3.14159f) * 0.3f;
                letters[i].transform.scale = { scaleFactor, scaleFactor, scaleFactor };

            } else {
                // 次の文字へ
                jumpTimer_ = 0.0f;
                letters[i].transform.rotate.y = 0.0f;
                letters[i].transform.translate.y = baseY;
                letters[i].transform.scale = { 0.6f,0.6f,0.6f }; 

                currentIndex_++;
                if (currentIndex_ >= NumLetters) {
                    currentIndex_ = 0;
                }
            }
        } else {
            // 回転していない文字は初期位置
            letters[i].transform.rotate.y = 0.0f;
            letters[i].transform.translate.y = baseY;
            letters[i].transform.scale = { 0.6f,0.6f,0.6f };
        }

        // 各文字に反映
        letters[i].obj->SetScale(letters[i].transform.scale);
        letters[i].obj->SetRotate(letters[i].transform.rotate);
        letters[i].obj->SetTranslate(letters[i].transform.translate);
        letters[i].obj->Update();
    }

    // PushSpace の点滅
    alphaTimer_ += DeltaTime;
    alpha_ = (sinf(alphaTimer_ * 3.0f) * 0.5f) + 0.5f;
    pushspace->SetColor({ 1.0f,1.0f,1.0f,alpha_ });
    pushspace->SetScale(pushspaceTransform.scale);
    pushspace->SetRotate(pushspaceTransform.rotate);
    pushspace->SetTranslate(pushspaceTransform.translate);
    pushspace->Update();
}

//描画
void GameClearObject::Draw() {
	//文字描画
    for (int i = 0; i < NumLetters; i++) {
        letters[i].obj->Draw();
    }
	//PushSpace描画
    pushspace->Draw();
}

//デバッグ
void GameClearObject::Debug() {
#ifdef USE_IMGUI
    if (ImGui::Begin("GameClearObject SRT")) {
        ImGui::Text("GameClear Letters");
        ImGui::Separator();

        for (int i = 0; i < NumLetters; i++) {
            std::string label = "Letter " + std::to_string(i);

            if (ImGui::TreeNode(label.c_str())) {
                ImGui::DragFloat3(("Scale##" + std::to_string(i)).c_str(), &letters[i].transform.scale.x, 0.01f);
                ImGui::DragFloat3(("Rotate##" + std::to_string(i)).c_str(), &letters[i].transform.rotate.x, 0.01f);
                ImGui::DragFloat3(("Translate##" + std::to_string(i)).c_str(), &letters[i].transform.translate.x, 0.01f);
                ImGui::TreePop();
            }
        }

        ImGui::Separator();
        ImGui::Text("Next");
        ImGui::DragFloat3("Next Scale", &pushspaceTransform.scale.x, 0.01f);
        ImGui::DragFloat3("Next Rotate", &pushspaceTransform.rotate.x, 0.01f);
        ImGui::DragFloat3("Next Translate", &pushspaceTransform.translate.x, 0.01f);
    }
    ImGui::End();
#endif
}
