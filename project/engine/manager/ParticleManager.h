#pragma once
#include "DirectXBase.h"
#include "SrvManager.h"
#include "Pso.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Struct.h"

#include <unordered_map>
#include <string>
#include <random>

//パーティクル種類
enum class ParticleType {
	Normal,//通常
	Ring,//Ring型
	Cylinder,//Cylinder型
	Explosive,//爆発
	Smoke,//煙
	PlayerMove,//プレイヤー移動
	EnemyDamage,//敵ダメージ
	Confetti,//紙吹雪
};

//パーティクル管理クラス
class ParticleManager
{
public:

	//パーティクル
	struct Particle {
		Transform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};
	//パーティクルの情報をGPUに送る
	struct ParticleForGPU {
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};
	//フィールド
	struct AccelerationField {
		Vector3 acceleration;//加速度
		AABB area;//範囲
	};
	//パーティクルグループ
	struct ParticleGroup {
		MaterialData materialData;//マテリアルデータ
		std::list<Particle>particles;//パーティクルリスト
		uint32_t SRVIndex;//インスタンシングデータ用SRVインデックス
		Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;//インスタンシングリソース
		uint32_t kNumInstance;//インスタンス数
		ParticleForGPU* instancingData ;//インスタンシングデータを書き込むためのポインタ
		ParticleType type;//パーティクルの種類
		ModelData modelData;
		Microsoft::WRL::ComPtr<ID3D12Resource>vertexResource;//頂点リソース
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	};
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// - 初回呼び出し時に内部でインスタンスを生成して返す。
	/// </summary>
	static ParticleManager* GetInstance();
	/// <summary>
	/// 初期化する。
	/// - DirectXBase / SrvManager / Camera ポインタを保存し、Pso を初期化してパーティクル用 PSO を作成する。
	/// - 乱数生成器を初期化し、マテリアル初期化やフィールドの既定値設定を行う。
	/// </summary>
	void Initialize(DirectXBase* directxBase, SrvManager* srvManager, Camera* camera);
	/// <summary>
	/// 終了処理を行う。
	/// - シングルトンインスタンスを破棄する（内部リソースは明示的にクリアされる想定）。
	/// </summary>
	void Finalize();
	/// <summary>
	/// シーン終了時に呼ぶクリア処理。
	/// - 登録済みのパーティクルグループを全て削除する。
	/// </summary>
	void Clear();
	/// <summary>
	/// 毎フレーム更新処理を行う。
	/// - カメラ行列を参照してビルボード行列を生成し、各グループのパーティクルを走査して時間経過・物理・インスタンシングデータの更新を行う。
	/// - 寿命を超えたパーティクルはリストから削除される。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - パーティクル用のルートシグネチャ／PSO をセットし、各グループについて頂点バッファ・マテリアル・テクスチャ・インスタンスSRV をバインドしてインスタンシング描画を実行する。
	/// </summary>
	void Draw();
	/// <summary>
	/// パーティクルグループを生成する。
	/// - 名前とテクスチャパス、タイプを指定してグループを登録する。
	/// - テクスチャ読み込み、モデル頂点生成（種類に応じて quad/ring/cylinder 等）、頂点バッファ・インスタンシングバッファの作成、SRV の割当てを行う。
	/// </summary>
	/// <param name="name">グループ名（識別キー）。</param>
	/// <param name="textureFilePath">使用するテクスチャのファイルパス。</param>
	/// <param name="type">パーティクルの種類。</param>
	void CreateparticleGroup(const std::string name, const std::string textureFilePath,ParticleType type);
	/// <summary>
	/// 新しいパーティクルを生成して返す。
	/// - 与えられた乱数エンジンと位置、タイプに基づいて transform / velocity / color / lifeTime をランダム化して初期化する。
	/// </summary>
	/// <param name="randomEngine">乱数エンジン（外部で一元管理された std::mt19937）。</param>
	/// <param name="translate">生成位置（ワールド座標）。</param>
	/// <param name="type">生成するパーティクルの種類。</param>
	/// <returns>初期化済みの Particle 構造体。</returns>
	Particle MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate, ParticleType type);
	/// <summary>
	/// 指定グループ名の位置にパーティクルを発生させる。
	/// - 指定個数分だけ MakeNewParticle を用いてパーティクルリストに追加する。
	/// </summary>
	/// <param name="name">グループ名。</param>
	/// <param name="position">発生位置。</param>
	/// <param name="count">発生させるパーティクル数。</param>
	void Emit(const std::string name, const Vector3& position, uint32_t count);
	/// <summary>
	/// 指定点が AABB の範囲内にあるか判定する。
	/// - 実装ではフィールド AABB と比較して包含判定を行う（true: 範囲内）。
	/// </summary>
	/// <param name="aabb">判定対象の AABB（参照）。</param>
	/// <param name="point">判定する点（ワールド座標）。</param>
	/// <returns>範囲内であれば true、そうでなければ false。</returns>
	bool IsCollision(const AABB& aabb, const Vector3& point);
public:
	/// <summary>
	/// 指定グループの ParticleType を取得する。
	/// - 存在しない名前を指定するとアサートで停止する実装。
	/// </summary>
	ParticleType GetParticleType(const std::string& name);
private:
	/// <summary>
	/// 汎用（四角）パーティクル用の頂点データを作成し modelData に格納する。
	/// - 頂点データ作成後、頂点バッファリソースを生成してコピーする処理を行う。
	/// </summary>
	void VertexDataCreate(ModelData&modelData);
	/// <summary>
	/// リング型パーティクルの頂点データを作成する。
	/// - 指定分割数で輪を分割し、頂点/UV/法線を modelData に詰める。
	/// </summary>
	void RingVertexDataCreate(ModelData&modelData);
	/// <summary>
	/// シリンダー型パーティクルの頂点データを作成する。
	/// - 円周分割して上面と底面のトライアングルを modelData に詰める。
	/// </summary>
	void CylinderVertexDataCreate(ModelData&modelData);
	/// <summary>
	/// マテリアル用の GPU バッファを生成して初期値を書き込む。
	/// - materialResource を生成し、color/enableLighting/uvTransform などを初期化する。
	/// </summary>
	void MaterialCreate();
private:

	static ParticleManager* instance;
	DirectXBase* directxBase_;
	SrvManager* srvManager_;
	std::unique_ptr<Pso>pso_;
	Camera* camera_;
	ModelData modelData;
	AABB aabb_;
	AccelerationField accelerationfield_;
	HRESULT hr;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	//乱数生成器の初期化
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
	//グループコンテナ
	std::unordered_map<std::string, ParticleGroup>particleGroups;

	const uint32_t kNumMaxInstance =128;
	//Δtを定義
	const float kDeltaTime = 1.0f / 60.0f;
	//billboardMatrix切り替え
	bool useBillboard = true;
};


