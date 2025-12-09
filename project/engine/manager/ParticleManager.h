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
    };

    // パーティクル管理クラス
    class ParticleManager {
    public:

        // パーティクル
        struct Particle {
            Transform transform;
            Vector3 velocity;
            Vector4 color;
            float life_time;
            float current_time;
        };

        // パーティクルの情報を GPU に送る
        struct ParticleForGPU {
            Matrix4x4 wvp;
            Matrix4x4 world;
            Vector4 color;
        };

        // フィールド
        struct AccelerationField {
            Vector3 acceleration;   // 加速度
            AABB area;              // 範囲
        };

        // パーティクルグループ
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
        struct ParticleInitData {
            Vector3 scale;
            Vector3 rotate;
            Vector3 velocity;
            Vector4 color;
            float life_time;
        };

    public:
        /// <summary>
        /// シングルトンインスタンスを取得する。
        /// - 初回呼び出し時に内部でインスタンスを生成して返す。
        /// </summary>
        static ParticleManager* GetInstance();

        /// <summary>
        /// 初期化する。
        /// - 各種ポインタを保存、PSO を生成し、マテリアル／乱数生成器を初期化する。
        /// </summary>
        void Initialize(DirectXBase* directx_base, SrvManager* srv_manager, Camera* camera);

        /// <summary>
        /// 終了処理を行う。
        /// - シングルトンインスタンスを破棄する。
        /// </summary>
        void Finalize();

        /// <summary>
        /// シーン終了時に呼ぶクリア処理。
        /// - 登録済みのパーティクルグループをすべて削除する。
        /// </summary>
        void Clear();

        /// <summary>
        /// 毎フレーム更新処理を行う。
        /// - 各パーティクルの寿命・物理更新、インスタンシングデータ更新を行う。
        /// </summary>
        void Update();

        /// <summary>
        /// 描画処理を行う。
        /// - 各グループについて頂点バッファ・マテリアル・テクスチャ・インスタンスSRV を設定して描画。
        /// </summary>
        void Draw();

        /// <summary>
        /// パーティクルグループを生成する。
        /// </summary>
        void CreateParticleGroup(const std::string name, const std::string texture_file_path, ParticleType type);

        /// <summary>
        /// 新しいパーティクルを生成して返す。
        /// </summary>
        Particle MakeNewParticle(std::mt19937& random_engine, const Vector3& translate, ParticleType type);

        /// <summary>
        /// 指定グループの位置にパーティクルを発生させる。
        /// </summary>
        void Emit(const std::string& name, const Vector3& position, uint32_t count);

        /// <summary>
        /// AABB 内に点が存在するか判定する。
        /// </summary>
        bool IsCollision(const AABB& aabb, const Vector3& point);

        /// <summary>
        /// 指定グループの ParticleType を取得する。
        /// </summary>
        ParticleType GetParticleType(const std::string& name);
    private:
        /// <summary>
        /// 汎用（四角）パーティクルの頂点データを作成し model_data に格納する。
        /// </summary>
        void VertexDataCreate(ModelData& model_data);

        /// <summary>
        /// リング型パーティクルの頂点データを作成する。
        /// </summary>
        void RingVertexDataCreate(ModelData& model_data);

        /// <summary>
        /// シリンダー型パーティクルの頂点データを作成する。
        /// </summary>
        void CylinderVertexDataCreate(ModelData& model_data);

        /// <summary>
        /// マテリアル用の GPU バッファを生成して初期値を書き込む。
        /// </summary>
        void MaterialCreate();

        /// <summary>
        /// 頂点バッファリソースを生成し、データをマッピング・コピーする。
        /// - 指定した頂点配列からGPUリソースを作成し、バッファビューと書き込みポインタをセットする。
        /// </summary>
        /// <param name="directx_base">DirectX基盤クラスのポインタ。</param>
        /// <param name="vertices">頂点データ配列。</param>
        /// <param name="vertex_resource">生成された頂点バッファリソース（出力）。</param>
        /// <param name="vertex_buffer_view">頂点バッファビュー（出力）。</param>
        /// <param name="vertex_data">マッピングされた頂点データ書き込み用ポインタ（出力）。</param>
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