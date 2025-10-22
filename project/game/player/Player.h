#pragma once

#include "Object3d.h"
#include "Object3dBase.h"
#include "Sprite.h"
#include "SpriteBase.h"
#include "Input.h"
#include "Struct.h"
#include "playerBullet.h"
#include "Camera.h"
#include <list>
#include <memory>
#include <vector>

//プレイヤー
class Player
{
public:
	/// <summary>
	/// コンストラクタ。
	/// - メンバのデフォルト初期化を行う。
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ。
	/// - 保持しているリソース（ユニークポインタ等）は自動で破棄される。
	/// </summary>
	~Player();
	/// <summary>
	/// 初期化する。
	/// - 引数の <c>Object3dBase*</c> を保持し、内部の <c>Object3d</c> を生成・初期化する。
	/// - モデルを "player/player.obj" に設定しライトを無効化、初期スケール・位置を設定する。
	/// - レティクル用スプライトを生成して初期設定する（サイズ・アンカーポイント等）。
	/// </summary>
	void Initialize(Object3dBase* object3dbase);
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 死亡時は早期リターンする。
	/// - クールダウン／タイマー類の更新、弾や内部オブジェクトの更新、移動・ジャンプ・回避・攻撃処理を順に行う。
	/// - レティクルの更新と弾リストの更新もここで行う。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - 死亡時は描画を行わない。
	/// - 内部の <c>Object3d</c> と保持する弾を描画する。
	/// </summary>
	void Draw();
	/// <summary>
	/// レティクル（照準）の描画を行う。
	/// - レティクル用スプライトの描画呼び出しを行う。
	/// </summary>
	void ReticleDraw();
	/// <summary>
	/// 移動処理を行う。
	/// - 入力（A/D）に応じた左右移動、座標クランプ（画面境界）を適用する。
	/// </summary>
	void Move();
	/// <summary>
	/// ジャンプ処理を行う。
	/// - 入力によりジャンプを開始し、重力・着地判定・二段ジャンプ管理を行う。
	/// </summary>
	void Jump();
	/// <summary>
	/// 攻撃（弾発射）を行う。
	/// - レティクルのスクリーン位置をワールド空間のレイに変換し、その方向へ弾を生成して発射する。
	/// - 攻撃クールタイム管理を行う。
	/// </summary>
	void Attack();
	/// <summary>
	/// 回避処理を行う。
	/// - 回避中は回転演出と時間管理を行い、クールダウンを適用する。
	/// - 回避コマンド（キー入力）で回避を開始する。
	/// </summary>
	void Dodge();
	/// <summary>
	/// レティクルの更新を行う。
	/// - 入力でスクリーン上のレティクル位置を移動し、スプライト位置を更新する。
	/// </summary>
	void ReticleUpdate();
	/// <summary>
	/// レティクルのスクリーン座標更新処理。
	/// - 必要に応じてスクリーン変換・制約を行う（実装は ReticleUpdate 内で呼ばれる想定）。
	/// </summary>
	void UpdateReticleScreenPos();
	/// <summary>
	/// 衝突時コールバック。
	/// - プレイヤーの死亡フラグを立てる等の処理を行う。
	/// </summary>
	void OnCollision();
	/// <summary>
	/// HP を減少させる。
	/// - 無敵時間や回避状態をチェックし、ダメージ適用後に無敵タイマー・色変化タイマーを設定する。
	/// - HP が 0 以下になったら <c>OnCollision()</c> を呼ぶ。
	/// </summary>
	/// <param name="damage">与えるダメージ量。</param>
	void TakeDamage(int damage);
    /// <summary>
	/// デバッグ表示を行う（ImGui を使用）。
	/// - HP / トランスフォーム / 回避状態などのインスペクションと一部操作を行える。
	/// </summary>
	void Debug();
	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する。
	/// - 現在の Transform とモデル寸法から OBB を計算して返す。
	/// </summary>
	/// <returns>計算された OBB。</returns>
	OBB GetOBB()const;
	/// <summary>
	/// 死亡フラグを取得する。
	/// </summary>
	/// <returns>デッドであれば true を返す。</returns>
	bool IsDead()const { return isDead_; }
public:
	///Getter/// 

	/// <summary>
	/// 現在のワールド座標（Transform.translate）を取得する。
	/// </summary>
	const Vector3& GetTranslate()const { return transform_.translate; }
	/// <summary>
	/// 衝突判定等で用いる半径を取得する。
	/// </summary>
	float GetRadius()const { return radius_; }
	/// <summary>
	/// 保持しているプレイヤー弾のリストを取得する（読み取り専用）。
	/// </summary>
	const std::list<std::unique_ptr<playerBullet>>& GetBullets() const { return bullets_; }

	///Setter/// 
	/// <summary>スケールを設定する。</summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	/// <summary>回転を設定する。</summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	/// <summary>座標を設定する。</summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }
	
private:
	Object3dBase* object3dBase_;
	Camera* camera_;
	Transform transform_;
	//レティクル用
	Transform reticleTransform_;
	std::unique_ptr<Object3d>object_;
	//弾のリスト
	std::list<std::unique_ptr<playerBullet>>bullets_;
	//レティクル
	std::unique_ptr<Sprite>reticle_;
	//画面上の位置
	Vector2 reticleScreenPos_{ 640.0f, 360.0f }; 
	Vector2 reticlePos_ = { 640.0f, 360.0f };
	//レティクルのオフセット
	Vector3 reticleOffset_{ 0.0f, 0.0f, 10.0f };
	//色
	Vector4 color_;
	//回避方向
	Vector3 dodgeDirection_{ 0.0f,0.0f,0.0f };
	//デスフラグ
	bool isDead_ = false;
	//回避状態
	bool dodge_ = false;
	//ジャンプフラグ
	bool isJumping_ = false;
	//無敵フラグ(デバック)
	bool isInvincible = false;
	//モデルの寸法
	float dimensions = 2.0f;
	//プレイヤーの移動速度
	float speed = 0.1f;
	//プレイヤーの半径
	float radius_ = 1.0f;
	//ジャンプ速度
	float jumpVelocity_ = 0.0f;
	
	//HP
	int hp_ = 100;
	//ジャンプ回数
	int jumpCount_ = 0;
	//最大ジャンプ可能回数
	const int maxJumpCount_ = 2;
	//弾の速度
	const float bulletSpeed = 1.0f;
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;
	//回避速度
	const float dodgeSpeed_ = 0.1f;
	//回避時の回転
	const float rotateAngle_ = 360.0f;
	//地面の最小Y移動
	const float groundminY = -3.0f;
	//地面の最小X移動
	const float groundminX = -4.0f;
	//地面の最大X移動
	const float groundmaxX = 4.0f;
	//重力
	const float gravity_ = -0.01f;
	//ジャンプ力
	const float jumpPower_ = 0.15f;
	//地面のY座標
	const float groundY_ = -1.5f;
	//二段ジャンプ回転速度
	const float jumpRotateSpeed_ = 720.0f;
	//色変化時間
	const float DamageColorDuration = 0.1f;
	//無敵時間
	const float InvincivleTime = 0.1f;
	//攻撃のクールタイム
	float  attackCooldown_ = 0.0f;
	//再攻撃できるまでの間隔
	float attackInterval_ = 10.0f;
	//回避タイマー
	float dodgeTimer_ = 0.0f;
	//回避適用時間
	float applyDodge_ = 0.5f;
	//回避のクールタイム
	float dodgeCooldown_ = 0.0f;
	//回避コマンド再使用待機時間
	float dodgeInterval_ = 5.0f;
	//ダメージ色変化タイマー
	float damageColorTimer_ = 0.0f;
	//無敵タイマー
	float invincibleTimer_ = 0.0f;
};

