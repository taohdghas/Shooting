#pragma once
#include "Object3d.h"
#include "Object3dBase.h"
#include "Transform.h"
#include "EnemyBullet.h"

// プレイヤークラス前方宣言
class Player;

// 敵の処理を書いたクラス
class Enemy {
public:
    /// <summary>
    /// 初期化する。
    /// - 引数の <c>Object3dBase*</c> を保持し、内部で <c>Object3d</c> を生成・初期化する。
    /// - モデルを "enemy/enemy.obj" に設定し、ライトを無効化、初期のスケール・位置を設定する実装を想定する。
    /// </summary>
    void Initialize(Object3dBase* object3d_base);

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
    void OnCollision();

    /// <summary>
    /// HP を減少させる。
    /// - ダメージに応じて HP を減らし、色変化タイマーをセットする。
    /// - HP が 0 以下になった場合は <c>OnCollision()</c> を呼んで死亡処理を行う。
    /// </summary>
    /// <param name="damage">与えるダメージ量。</param>
    void TakeDamage(int damage);

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
    OBB GetOBB() const;

    /// <summary>
    /// 死亡フラグが立っているか判定する。
    /// </summary>
    /// <returns>死亡していれば true。</returns>
    bool IsDead() const { return is_dead_; }

    /// <summary>
    /// 死亡時のパーティクル発生フラグが立っているか判定する。
    /// </summary>
    /// <returns>デスパーティクル発生フラグが立っていれば true。</returns>
    bool IsDeathParticle() const { return is_death_particle_; }

public:
    /// <summary>
    /// 現在の位置（Transform.translate）を取得する。
    /// </summary>
    const Vector3& GetPosition() const { return transform_.translate; }

    /// <summary>
    /// コリジョンや描画に用いる半径を取得する。
    /// </summary>
    float GetRadius() const { return radius_; }

    /// <summary>
    /// 所有する弾リストを取得する（読み取り専用）。
    /// </summary>
    const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

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
    void SetIsDeathParticle(bool flag) { is_death_particle_ = flag; }

    /// <summary>
    /// 敵の移動用レール（スプライン）の制御点と閉じているかどうかを設定する。
    /// - 引数の制御点リストを内部に保持し、レールの閉じている状態フラグも設定する。
    /// - レールに沿った移動やパス制御に利用される。
    /// </summary>
    /// <param name="controlPoints">レールの制御点（ワールド座標の Vector3 配列）。</param>
    /// <param name="closed">レールが閉じているか（true:閉じている/false:開いている）。</param>
    void SetRail(const std::vector<Vector3>& controlPoints, bool closed);
private:
    Object3dBase* object3d_base_;
    std::unique_ptr<Object3d> object_;
    Transform transform_;
    Player* player_;
    std::list<std::unique_ptr<EnemyBullet>> bullets_;
    std::vector<Vector3> railPoints_;  
    Vector3 velocity_ = { 0.0f, 0.0f };
    Vector4 color_;
    bool is_dead_ = false;
    bool is_death_particle_ = false;
    int hp_ = 100;
    int fire_timer_count_ = 0;
    int fire_timer_ = 0;
    float radius_ = 1.0f;
    float dimensions_ = 2.0f;
    static const int k_fire_interval_ = 80;
    const float fire_distance_ = 25.0f;
    const float attack_stop_distance_z_ = 6.0f;
    const float delta_time_ = 1.0f / 60.0f;
    const float damage_color_duration_ = 0.1f;
    float damage_color_timer_ = 0.0f;
    bool railClosed_ = false;         
    float railProgress_ = 0.0f;      
    float railSpeed_ = 1.0f;           
};
