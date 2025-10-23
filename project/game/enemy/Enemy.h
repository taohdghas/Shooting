#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"

//プレイヤークラス前方宣言
class Player;
//敵の処理を書いたクラス
class Enemy
{
public:
	/// <summary>
	/// 初期化する。
	/// - 引数の <c>Object3dBase*</c> を保持し、内部で <c>Object3d</c> を生成・初期化する。
	/// - モデルを "enemy/enemy.obj" に設定し、ライトを無効化、初期のスケール・位置を設定する実装を想定する。
	/// </summary>
	void Initialize(Object3dBase*object3dBase);
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 死亡フラグが立っていれば処理をスキップする。
	/// - 所有する弾の更新と不要弾の削除を行う。
	/// - 移動・攻撃（<c>Laser()</c> 呼び出し）を行い、Transform を内部の <c>Object3d</c> に反映する。
	/// - ダメージ色変化タイマーの更新と色の切り替えを行い、内部オブジェクトを Update する。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画を行う。
	/// - 死亡フラグが立っていれば描画を行わない。
	/// - 内部の <c>Object3d</c> と所有する弾を描画する。
	/// </summary>
	void Draw();
	/// <summary>
	/// レーザー攻撃（弾発射）を行う。
	/// - プレイヤーへの方向・距離・タイマー等をチェックし、発射条件を満たす場合に弾を生成してリストに追加する。
	/// </summary>
	void Laser();
	/// <summary>
	/// 衝突時のコールバック。
	/// - デスフラグやデスパーティクル発生フラグをセットする。
	/// </summary>
	void onCollision();
	/// <summary>
	/// HP を減少させる。
	/// - ダメージに応じて HP を減らし、色変化タイマーをセットする。
	/// - HP が 0 以下になった場合は <c>onCollision()</c> を呼んで死亡処理を行う。
	/// </summary>
	/// <param name="damege">与えるダメージ量。</param>
	void TakeDamage(int damege);
	/// <summary>
	/// デバッグ UI を表示する（ImGui を使用）。
	/// - ID に基づく一意のラベルを生成して HP / スケール / 回転 / 平行移動の編集ウィジェットを表示する実装を想定する。
	/// </summary>
	/// <param name="id">デバッグ UI 用の識別子。</param>
	void Debug(int id);
	/// <summary>
	/// OBB（Oriented Bounding Box）を取得する。
	/// - 現在の Transform（位置・回転・スケール）と内部寸法を基に OBB を構築して返す。
	/// </summary>
	/// <returns>計算された OBB。</returns>
	OBB GetOBB()const;
	/// <summary>
	/// 死亡フラグが立っているか判定する。
	/// </summary>
	/// <returns>死亡していれば true。</returns>
	bool IsDead()const { return isDead_; }
	/// <summary>
	/// 死亡時のパーティクル発生フラグが立っているか判定する。
	/// </summary>
	/// <returns>デスパーティクル発生フラグが立っていれば true。</returns>
	bool IsDeathParticle()const { return isDeathParticle_; }

public:
	/// <summary>
	/// 現在の位置（Transform.translate）を取得する。
	/// </summary>
	const Vector3& GetPosition()const { return transform_.translate; }
	/// <summary>
	/// コリジョンや描画に用いる半径を取得する。
	/// </summary>
	float GetRadius()const { return radius_; }
	/// <summary>
	/// 所有する弾リストを取得する（読み取り専用）。
	/// </summary>
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()const { return bullets_; }

	/// <summary>
	/// スケールを設定する。
	/// </summary>
	void SetScale(const Vector3& scale) { transform_.scale = scale; }
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	void SetTranslate(const Vector3& position) { transform_.translate = position; }
	/// <summary>
	/// 対象プレイヤーのポインタを設定する。
	/// - 攻撃判定や追従に利用される。
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// デスパーティクル発生フラグを設定する。
	/// </summary>
	void SetisDeathParticle(bool flag) { isDeathParticle_ = flag; }
private:
	Object3dBase* object3dBase_;
	std::unique_ptr<Object3d>object_;
	Transform transform_;
	//プレイヤー
	Player*player_;
	//弾のリスト
	std::list<std::unique_ptr<EnemyBullet>>bullets_;
	//移動速度
	Vector3 velocity_ = { 0.0,0 };
	//色
	Vector4 color_;
	//デスフラグ
	bool isDead_ = false;
	//デスパーティクル発生フラグ
	bool isDeathParticle_ = false;
	//HP
	int hp_ = 100;
	//レーザー発射カウントタイマー
	int fireTimerCount_ = 0;
	//発射タイマー
	int fireTimer_ = 0;
	//半径
	float radius_ = 1.0f;
	//モデルの寸法
	float dimensions = 2.0f;
	//発射間隔
	static const int kFireInterval = 80;
	//発射距離
	const float fireDistance = 25.0f;
	//攻撃停止距離Z
	const float attackStopDisntanceZ = 6;
	//Δtを定義
	const float DeltaTime = 1.0f / 60.0f;
	//色変化時間
	const float damageColorDuration = 0.1f;
	//ダメージ色変化タイマー
	float damageColorTimer_ = 0.0f;
};

