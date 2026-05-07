#pragma once
#include "DirectXBase.h"
#include "SrvManager.h"
#include "PipeLineStateObject.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Struct.h"

#include <unordered_map>
#include <string>
#include <random>
#include <unordered_map>

namespace MyEngine {

    // パーティクル種類
    enum class ParticleType {
        Normal,      // 通常
        Ring,        // Ring型
        Cylinder,    // Cylinder型
        Explosive,   // 爆発
        Smoke,       //煙
    };

    // パーティクル管理クラス
    /// <summary>
    /// 複数種類のパーティクルの生成・管理・描画
    /// パーティクルグループごとのリソースの生成・管理
    /// パーティクルの発生・更新・消滅・物理挙動の制御
    /// GPUへのパーティクルデータ転送およびインスタンシング描画の実装
    /// DirectX/カメラ/テクスチャ/パイプライン等の外部リソースとの連携
    /// シングルトンによるインスタンス管理
    /// </summary>
    class ParticleManager {
    public:

        // パーティクル
        /// <summary>
        /// パーティクルの変換情報・速度・色・寿命を保持する構造体
        /// </summary>
        struct Particle {
            Transform transform;
            Vector3 velocity;
            Vector4 color;
            float life_time;
            float current_time;
        };

        // パーティクルの情報を GPU に送る
        /// <summary>
        /// パーティクルのワールド変換行列や色をGPUに送るための構造体
        /// </summary>
        struct ParticleForGPU {
            Matrix4x4 wvp;
            Matrix4x4 world;
            Vector4 color;
        };

        // フィールド
        /// <summary>
        /// パーティクルに影響を与える加速度と範囲を持つ構造体
        /// </summary>
        struct AccelerationField {
            Vector3 acceleration;   // 加速度
            AABB area;              // 範囲
        };

        // パーティクルグループ
        /// <summary>
        /// パーティクルのマテリアルやリソース・リスト・種類などをまとめた構造体
        /// </summary>
        struct ParticleGroup {
            MaterialData material_data;                           // マテリアルデータ
            std::list<Particle> particles;                        // パーティクルリスト
            uint32_t srv_index;                                   // インスタンシングデータ用 SRV インデックス
            Microsoft::WRL::ComPtr<ID3D12Resource> instancing_resource; // インスタンシングリソース
            uint32_t num_instance;                                // インスタンス数
            ParticleForGPU* instancing_data;                      // インスタンシングデータ書き込みポインタ
            ParticleType type;                                    // パーティクル種類
            ModelData model_data;
            Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource;
            D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view;
        };

        // パーティクル初期値データ
        /// <summary>
        /// パーティクルの初期スケール・回転・速度・色・寿命をまとめた構造体
        /// </summary>
        struct ParticleInitData {
            Vector3 scale;
            Vector3 rotate;
            Vector3 velocity;
            Vector4 color;
            float life_time;
        };

    public:
        /// <summary>
        /// シングルトンインスタンスを取得
        /// <returns>ParticleManagerのインスタンス（ポインタ）</returns>
        /// </summary>
        static ParticleManager* GetInstance();

        /// <summary>
        /// 初期化
        /// <param name="directx_base">DirectXBaseクラスのポインタ</param>
        /// <param name="srv_manager">SrvManagerクラスのポインタ</param>
        /// <param name="camera">Cameraクラスのポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void Initialize(DirectXBase* directx_base, SrvManager* srv_manager, Camera* camera);

        /// <summary>
        /// 終了処理
        /// <returns>なし</returns>
        /// </summary>
        void Finalize();

        /// <summary>
        /// シーン終了時に呼ぶクリア処理
        /// <returns>なし</returns>
        /// </summary>
        void Clear();

        /// <summary>
        /// 毎フレーム更新処理
        /// <returns>なし</returns>
        /// </summary>
        void Update();

        /// <summary>
        /// 描画処理
        /// <returns>なし</returns>
        /// </summary>
        void Draw();

        /// <summary>
        /// パーティクルグループを生成
        /// <param name="name">グループ名（std::string）</param>
        /// <param name="texture_file_path">テクスチャファイルパス（std::string）</param>
        /// <param name="type">パーティクル種類（ParticleType）</param>
        /// <returns>なし</returns>
        /// </summary>
        void CreateParticleGroup(const std::string name, const std::string texture_file_path, ParticleType type);

        /// <summary>
        /// 新しいパーティクルを生成して返す
        /// <param name="random_engine">乱数エンジン</param>
        /// <param name="translate">生成位置（Vector3）</param>
        /// <param name="type">パーティクル種類（ParticleType）</param>
        /// <returns>生成されたParticle構造体</returns>
        /// </summary>
        Particle MakeNewParticle(std::mt19937& random_engine, const Vector3& translate, ParticleType type);

        /// <summary>
        /// 指定グループの位置にパーティクルを発生
        /// <param name="name">グループ名（std::string）</param>
        /// <param name="position">発生位置（Vector3）</param>
        /// <param name="count">発生数（uint32_t）</param>
        /// <returns>なし</returns>
        /// </summary>
        void Emit(const std::string& name, const Vector3& position, uint32_t count);

        /// <summary>
        /// AABB 内に点が存在するか判定
        /// <param name="aabb">AABB構造体</param>
        /// <param name="point">判定する点（Vector3）</param>
        /// <returns>存在すればtrue、そうでなければfalse</returns>
        /// </summary>
        bool IsCollision(const AABB& aabb, const Vector3& point);

        /// <summary>
        /// 指定グループの ParticleType を取得
        /// <param name="name">グループ名（std::string）</param>
        /// <returns>パーティクル種類（ParticleType）</returns>
        /// </summary>
        ParticleType GetParticleType(const std::string& name);

    private:
        /// <summary>
        /// 四角パーティクルの頂点データを作成し model_data に格納
        /// <param name="model_data">頂点データを格納するModelData構造体</param>
        /// <returns>なし</returns>
        /// </summary>
        void VertexDataCreate(ModelData& model_data);

        /// <summary>
        /// リング型パーティクルの頂点データを作成
        /// <param name="model_data">頂点データを格納するModelData構造体</param>
        /// <returns>なし</returns>
        /// </summary>
        void RingVertexDataCreate(ModelData& model_data);

        /// <summary>
        /// シリンダー型パーティクルの頂点データを作成
        /// <param name="model_data">頂点データを格納するModelData構造体</param>
        /// <returns>なし</returns>
        /// </summary>
        void CylinderVertexDataCreate(ModelData& model_data);

        /// <summary>
        /// マテリアル用の GPU バッファを生成して初期値を書き込む
        /// <returns>なし</returns>
        /// </summary>
        void MaterialCreate();

        /// <summary>
        /// 頂点バッファリソースを生成し、データをマッピング・コピー
        /// <param name="directx_base">DirectXBaseクラスのポインタ</param>
        /// <param name="vertices">頂点データ配列</param>
        /// <param name="vertex_resource">生成された頂点リソース</param>
        /// <param name="vertex_buffer_view">頂点バッファビュー</param>
        /// <param name="vertex_data">頂点データ書き込み用ポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
        void CreateAndMapVertexBuffer(
            DirectXBase* directx_base,
            const std::vector<VertexData>& vertices,
            Microsoft::WRL::ComPtr<ID3D12Resource>& vertex_resource,
            D3D12_VERTEX_BUFFER_VIEW& vertex_buffer_view,
            VertexData*& vertex_data);
    private:
        static std::unique_ptr<ParticleManager> instance;
        // ParticleTypeごとの初期値テーブル
        static const std::unordered_map<ParticleType, ParticleInitData> kParticleInitTable;

        DirectXBase* directx_base_;
        SrvManager* srv_manager_;
        std::unique_ptr<PipelineStateObject> pso_;
        Camera* camera_;
        ModelData model_data_;
        AABB aabb_;
        AccelerationField acceleration_field_;
        HRESULT hr_;
        // バッファリソース内データのポインタ
        VertexData* vertex_data_ = nullptr;
        Material* material_data_ = nullptr;

        // バッファビュー
        D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

        // リソース
        Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;
        Microsoft::WRL::ComPtr<ID3D12Resource> material_resource_;

        // 乱数生成器
        std::random_device seed_generator_;
        std::mt19937 random_engine_;

        // グループコンテナ
        std::unordered_map<std::string, ParticleGroup> particle_groups_;

        // 定数（Google C++ Style：k + PascalCase）
        static constexpr uint32_t kNumMaxInstance = 128;
        static constexpr float kDeltaTime = 1.0f / 60.0f;

        // ビルボード切り替え
        bool use_billboard_ = true;
    };
}