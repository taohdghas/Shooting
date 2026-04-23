#include "Ui.h"
#include "SpriteBase.h"
#include "Input.h"
#include "MyMath.h"
#include "ImGuiManager.h"
#include <algorithm>

//初期化
void Ui::Initialize()
{
	//ゲームスタート作成
	CreateSprite(SpriteType::GameStart, "resources/ui/game_start.png", { -360.0f,360.0f }, { 720.0f,120.0f }, { 0.5f,0.5f });
	//HPバー作成
	CreateSprite(SpriteType::HpBar, "resources/ui/hpbar.png", { 50,50 }, { 200,20 });
	//HPバーフラッシュ作成
	CreateSprite(SpriteType::HpBarFlash, "resources/ui/hpbar_flash.png", { 50,50 }, { 200,20 });
	//HPバー注意作成
	CreateSprite(SpriteType::HpBarCaution, "resources/ui/hpbarcaution.png", { 50,50 }, { 200,20 });
	//HPバー注意フラッシュ作成
	CreateSprite(SpriteType::HpBarCautionFlash, "resources/ui/hpbarcaution_flash.png", { 50,50 }, { 200,20 });
	//HPバー危険作成
	CreateSprite(SpriteType::HpBarDanger, "resources/ui/hpbardanger.png", { 50,50 }, { 200,20 });
	//HPバー危険フラッシュ作成
	CreateSprite(SpriteType::hpBarDangerFlash, "resources/ui/hpbardanger_flash.png", { 50,50 }, { 200,20 });
	//回避ゲージ作成
	CreateSprite(SpriteType::DodgeGauge, "resources/ui/dodgecooldownbar.png", { 50,80 }, { 200,20 });
	//回避ゲージフラッシュ作成
	CreateSprite(SpriteType::DodgeGaugeFlash, "resources/ui/dodgecooldownbar_flash.png", { 50,80 }, { 200,20 });
	//ポーズ文字作成
	CreateSprite(SpriteType::Pause, "resources/ui/pause.png", { 1000,20 }, { 300,60 });
	//リトライ文字作成
	CreateSprite(SpriteType::Retry, "resources/ui/retry.png", { 485,620 }, { 180,100 }, { 0.5f,0.5f });
	//タイトルへ戻る文字作成
	CreateSprite(SpriteType::BackTitle, "resources/ui/back_title.png", { 805,620 }, { 180,100 }, { 0.5f,0.5f });
	//操作説明画面作成
	CreateSprite(SpriteType::OperationGuide, "resources/ui/operation.png", { 640,300 }, { 500,500 }, { 0.5f,0.5f });
	//各キー説明作成
	CreateSprite(SpriteType::KeyA, "resources/ui/keyboard_a.png", { 440,500 }, { 50,50 });
	//Dキー
	CreateSprite(SpriteType::KeyD, "resources/ui/keyboard_d.png", { 540,500 }, { 50,50 });
	//Fキー
	CreateSprite(SpriteType::KeyF, "resources/ui/keyboard_f.png", { 640,500 }, { 50,50 });
	//Wキー
	CreateSprite(SpriteType::KeyW, "resources/ui/keyboard_w.png", { 490,450 }, { 50,50 });
	//マウス左
	CreateSprite(SpriteType::MouseLeft, "resources/ui/mouse_left.png", { 740,500 }, { 50,50 });
	//マウス移動
	CreateSprite(SpriteType::MouseMove, "resources/ui/mouse_move.png", { 840,500 }, { 50,50 });
}
//更新
void Ui::Update()
{
	//ゲームスタート演出更新
	UpdateGameStart();
	//HPバー更新
	UpdateHPBar();
	//操作説明画面更新
	UpdatePauseGuide();
	//ポーズボタンホバー更新
	UpdatePauseButtonHover();
	//ポーズメニュークリック更新
	UpdatePauseClick();
	//回避ゲージ更新
	UpdateDodgeGauge();

	//スプライト更新
	for (auto& [type, sprite] : sprites_) {
		sprite->Update();
	}
}
//描画
void Ui::Draw()
{
	//ゲームスタート文字
	if (game_start_state_ != GameStartState::None)
	{
		Get(SpriteType::GameStart)->Draw();
	}
	//HPバー描画
	if (is_hp_flash_visible_) {
		Get(current_hp_flash_)->Draw();
	} else {
		Get(current_hp_bar_)->Draw();
	}

	// 回避ゲージ
	if (!is_dodge_flash_visible_) {
		Get(SpriteType::DodgeGaugeFlash)->Draw();
	} else {
		Get(SpriteType::DodgeGauge)->Draw();
	}

	// ポーズ文字
	Get(SpriteType::Pause)->Draw();

	// ポーズメニュー
	if (pause_scale_ > 0.01f) {
		Get(SpriteType::Retry)->Draw();
		Get(SpriteType::BackTitle)->Draw();
		Get(SpriteType::OperationGuide)->Draw();
	}
}
//デバック
void Ui::Debug()
{
#ifdef USE_IMGUI
	if (ImGui::Begin("UI Debug")) {

		for (auto& [type, sprite] : sprites_) {

			std::string name = std::to_string(static_cast<int>(type));
			if (ImGui::TreeNode(name.c_str())) {

				Vector2 pos = sprite->GetPosition();
				Vector2 size = sprite->GetSize();
				float rot = sprite->GetRotation();

				ImGui::DragFloat2("Position", &pos.x, 1.0f);
				ImGui::DragFloat2("Size", &size.x, 1.0f);
				ImGui::DragFloat("Rotation", &rot, 0.01f);

				sprite->SetPosition(pos);
				sprite->SetSize(size);
				sprite->SetRotation(rot);

				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
#endif
}
//ゲームスタート演出更新
void Ui::UpdateGameStart()
{
	switch (game_start_state_)
	{
		//スライドイン
	case GameStartState::SlideIn:

		game_start_animation_time_ += 1.0f / 60.0f;

		{
			float t = min(game_start_animation_time_ / kSlideDuration, 1.0f);
			float eased = (float)Math::easeOutQuad(t);

			game_start_pos_.x =
				slide_start_x_ + (slide_end_x_ - slide_start_x_) * eased;

			Get(SpriteType::GameStart)->SetPosition(game_start_pos_);

			//アニメーション終了判定
			if (t >= 1.0f)
			{
				game_start_state_ = GameStartState::Stop;
				game_start_timer_ = 0.0f;
			}
		}

		break;

		//停止
	case GameStartState::Stop:

		game_start_timer_ += 1.0f / 60.0f;

		if (game_start_timer_ >= kStopTime)
		{
			game_start_animation_time_ = 0.0f;

			slide_start_x_ = screen_center_x;
			slide_end_x_ = 1600.0f;

			game_start_state_ = GameStartState::SlideOut;
		}

		break;

		//スライドアウト
	case GameStartState::SlideOut:

		game_start_animation_time_ += 1.0f / 60.0f;

		{
			float t = min(game_start_animation_time_ / kSlideDuration, 1.0f);
			float eased = (float)Math::easeInOutQuad(t);

			game_start_pos_.x =
				slide_start_x_ + (slide_end_x_ - slide_start_x_) * eased;

			Get(SpriteType::GameStart)->SetPosition(game_start_pos_);
			//アニメーション終了判定
			if (t >= 1.0f)
			{
				game_start_state_ = GameStartState::None;
			}
		}

		break;

	default:
		break;
	}
}

//HPバー更新
void Ui::UpdateHPBar()
{
	//ポーズ画面が開いているならスキップ
	if (is_show_pause_)return;

	if (!player_) return;

	//実際のHP比率
	float currentHP = static_cast<float>(player_->GetHP());
	constexpr float maxHP = 100.0f;
	float targetRatio = std::clamp(currentHP / maxHP, 0.0f, 1.0f);

	// HP帯でバーの切り替え
	if (targetRatio > 0.7f) {
		current_hp_bar_ = SpriteType::HpBar;
		current_hp_flash_ = SpriteType::HpBarFlash;
	} else if (targetRatio > 0.4f) {
		current_hp_bar_ = SpriteType::HpBarCaution;
		current_hp_flash_ = SpriteType::HpBarCautionFlash;
	} else {
		current_hp_bar_ = SpriteType::HpBarDanger;
		current_hp_flash_ = SpriteType::hpBarDangerFlash;
	}

	//HP減少じバー点滅
	if (targetRatio < prev_hp_ratio_) {
		hp_flash_timer_ = 0.0f;
		is_hp_flash_visible_ = true;
	}

	//点滅制御
	if (is_hp_flash_visible_) {
		hp_flash_timer_ += 1.0f / 60.0f;
		if (hp_flash_timer_ > 0.2f) {
			hp_flash_timer_ = 0.0f;
			is_hp_flash_visible_ = false;
		}
	}

	//HP比率を補間 
	float speed =
		(display_hp_ratio_ > targetRatio) ? 0.2f : 0.05f;

	display_hp_ratio_ += (targetRatio - display_hp_ratio_) * speed;

	// 誤差防止
	display_hp_ratio_ = std::clamp(display_hp_ratio_, 0.0f, 1.0f);

	//サイズ更新
	Vector2 size = { 200.0f * display_hp_ratio_, 20.0f };

	Get(SpriteType::HpBar)->SetSize(size);
	Get(SpriteType::HpBarFlash)->SetSize(size);
	Get(SpriteType::HpBarCaution)->SetSize(size);
	Get(SpriteType::HpBarCautionFlash)->SetSize(size);
	Get(SpriteType::HpBarDanger)->SetSize(size);
	Get(SpriteType::hpBarDangerFlash)->SetSize(size);


	//前フレームのHP比率保存
	prev_hp_ratio_ = targetRatio;
}

//操作説明画面処理
void Ui::UpdatePauseGuide()
{
	//スケール変化速度
	const float speed = 0.07f;
	//スケール更新
	if (is_show_pause_) {
		pause_scale_ += speed;
	} else {
		pause_scale_ -= speed;
	}
	//範囲制限
	pause_scale_ = std::clamp(pause_scale_, 0.0f, 1.0f);
	//イージング適用
	float eased = static_cast<float>(Math::easeOutQuad(pause_scale_));
	//リトライ
	Get(SpriteType::Retry)->SetSize(Math::MultiplyScalar(retry_base_size_, eased));
	//タイトルへ戻る
	Get(SpriteType::BackTitle)->SetSize(Math::MultiplyScalar(back_title_base_size_, eased));
	//操作説明画面
	Get(SpriteType::OperationGuide)->SetSize(Math::MultiplyScalar(operation_base_size_, eased));
}

//ポーズボタンホバー処理
void Ui::UpdatePauseButtonHover()
{
	if (pause_scale_ <= 0.01f) return;
	//ホバータイマー更新
	hover_timer_ += 1.0f / 60.0f;

	float animationScale = 1.0f + sinf(hover_timer_ * 5.0f) * 0.1f;

	float eased = static_cast<float>(Math::easeOutQuad(pause_scale_));

	Vector2 retrySize = Math::MultiplyScalar(retry_base_size_, eased);
	Vector2 backSize = Math::MultiplyScalar(back_title_base_size_, eased);

	// 通常サイズに戻す
	Get(SpriteType::Retry)->SetSize(retrySize);
	Get(SpriteType::BackTitle)->SetSize(backSize);

	// リトライボタン
	if (IsMouseOnSprite(Get(SpriteType::Retry))) {
		Get(SpriteType::Retry)->SetSize(Math::MultiplyScalar(retrySize, animationScale));
	}
	// タイトルへ戻るボタン
	if (IsMouseOnSprite(Get(SpriteType::BackTitle))) {
		Get(SpriteType::BackTitle)->SetSize(Math::MultiplyScalar(backSize, animationScale));
	}
}
//ポーズメニュークリック処理
void Ui::UpdatePauseClick()
{
	//ポーズ画面が出ていないならスキップ
	if (!is_show_pause_) return;
	//スケールが十分でないならスキップ
	if (pause_scale_ < 0.95f) return;

	auto input = MyEngine::Input::GetInstance();
	//マウス左クリックされていなければスキップ
	if (!input->IsMouseLeftTriggered()) return;

	if (IsMouseOnSprite(Get(SpriteType::Retry))) {
		pause_result_ = PauseResult::Retry;
	} else if (IsMouseOnSprite(Get(SpriteType::BackTitle))) {
		pause_result_ = PauseResult::BackTitle;
	}
}
//回避クールタイムバー更新
void Ui::UpdateDodgeGauge()
{
	//ポーズ画面が開いているならスキップ
	if (is_show_pause_)return;

	if (!player_) return;

	float ratio = player_->GetDodgeCooldownRatio();
	constexpr Vector2 maxSize = { 200.0f, 20.0f };

	Vector2 size = { maxSize.x * ratio, maxSize.y };

	Get(SpriteType::DodgeGauge)->SetSize(size);
	Get(SpriteType::DodgeGaugeFlash)->SetSize(size);

	//点滅制御
	if (ratio < 1.0f) {
		//クールタイム中交互にスプライト表示
		dodge_flash_timer_ += kDeltaTime;
		//一定時間ごとに表示非表示切り替え
		if (dodge_flash_timer_ > kDodgeFlashDuration) {
			dodge_flash_timer_ = 0.0f;
			is_dodge_flash_visible_ = !is_dodge_flash_visible_;
		}
	} else {
		// 満タン → 強調表示固定
		is_dodge_flash_visible_ = true;
	}
}

//スプライト作成
void Ui::CreateSprite(SpriteType type,
	const char* path,
	const Vector2& pos,
	const Vector2& size,
	const Vector2& anchor)
{
	auto sprite = std::make_unique<MyEngine::Sprite>();
	sprite->Initialize(MyEngine::SpriteBase::GetInstance(), path);
	sprite->SetPosition(pos);
	sprite->SetSize(size);
	sprite->SetAnchorPoint(anchor);

	sprites_[type] = std::move(sprite);
}
//ゲームスタートアニメーション開始
void Ui::StartGameStartAnimation()
{
	
	has_started_game_start_ = true;
	//アニメーション状態初期化
	game_start_state_ = GameStartState::SlideIn;
	//タイマー初期化
	game_start_timer_ = 0.0f;
	game_start_animation_time_ = 0.0f;
	//スライドの開始・終了位置設定
	slide_start_x_ = -360.0f;
	slide_end_x_ = screen_center_x;
	//ゲームスタート文字の初期位置設定
	game_start_pos_ = { slide_start_x_,360.0f };

	Get(SpriteType::GameStart)->SetPosition(game_start_pos_);
}

//スプライト上にマウスが乗っているか
bool Ui::IsMouseOnSprite(MyEngine::Sprite* sprite)
{
	if (!sprite) return false;

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

///スプライト取得
MyEngine::Sprite* Ui::Get(SpriteType type)
{
	auto it = sprites_.find(type);
	if (it == sprites_.end()) return nullptr;
	return it->second.get();
}