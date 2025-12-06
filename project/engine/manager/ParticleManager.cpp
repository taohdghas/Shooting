#include "ParticleManager.h"
#include "MyMath.h"
#include "CameraManager.h"
#include <assert.h>
#include <numbers>

std::unique_ptr<ParticleManager> ParticleManager::instance = nullptr;
//シングルトンインスタンスの取得
ParticleManager* ParticleManager::GetInstance() {
	if (!instance) {
		instance = std::make_unique<ParticleManager>();
	}
	return instance.get();
}

void ParticleManager::Initialize(DirectXBase* directx_base, SrvManager* srv_manager, Camera* camera) {
	this->directx_base_ = directx_base;
	this->srv_manager_ = srv_manager;
	pso_ = std::make_unique<PipelineStateObject>();
	pso_->Initialize(directx_base_);
	this->camera_ = camera;
	// 乱数生成器
	random_engine_.seed(seed_generator_());
	// グラフィックスパイプライン
	pso_->CreateParticlePipelineState();
	// マテリアルデータ
	MaterialCreate();
	// Field
	acceleration_field_.acceleration = { 15.0f,0.0f,0.0f };
	acceleration_field_.area.min = { -1.0f,-1.0f,-1.0f };
	acceleration_field_.area.max = { 1.0f,1.0f,1.0f };
}
// 終了
void ParticleManager::Finalize() {
	instance.reset();
}

// シーン終了時に呼ぶ
void ParticleManager::Clear() {
	particle_groups_.clear();
}
//更新
void ParticleManager::Update() {
	Matrix4x4 camera_matrix = CameraManager::GetInstance()->GetActiveCamera()->GetWorldMatrix();
	Matrix4x4 back_to_front_matrix = Math::MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboard_matrix = Math::Multiply(back_to_front_matrix, camera_matrix);

	// ビュー行列
	Matrix4x4 view_matrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewMatrix();
	// プロジェクション行列
	Matrix4x4 projection_matrix = CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix();
	// ビュープロジェクション行列
	Matrix4x4 view_projection_matrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();

	// true なら使う
	if (use_billboard_) {
		billboard_matrix.m[3][0] = 0.0f;
		billboard_matrix.m[3][1] = 0.0f;
		billboard_matrix.m[3][2] = 0.0f;
	}
	// false なら単位行列
	else if (!use_billboard_) {
		billboard_matrix = Math::MakeIdentity4x4();
	}

	for (auto& particle_group_pair : particle_groups_) {
		auto& group = particle_group_pair.second;
		group.num_instance = 0;

		for (auto particle_it = group.particles.begin(); particle_it != group.particles.end();) {

			// 時間更新
			(*particle_it).current_time += kDeltaTime;

			// 寿命経過
			if ((*particle_it).life_time <= (*particle_it).current_time) {
				particle_it = group.particles.erase(particle_it);
				continue;
			}

			float alpha = 1.0f - ((*particle_it).current_time / (*particle_it).life_time);

			if (group.num_instance < kNumMaxInstance) {

				// Cylinder 回転
				if (group.type == ParticleType::Cylinder) {
					(*particle_it).transform.rotate.y += 1.0f * kDeltaTime;
				}

				// Explosive 処理
				if (group.type == ParticleType::Explosive) {
					float t = (*particle_it).current_time / (*particle_it).life_time;

					if (t < 0.5f) {
						float scale = 1.0f - t * 2.0f;
						scale = max(scale, 0.05f);
						(*particle_it).transform.scale = { scale, scale, scale };
					} else {
						std::uniform_real_distribution<float> dist_scale(0.1f, 0.35f);
						float random_scale = dist_scale(random_engine_);
						(*particle_it).transform.scale = { random_scale, random_scale, random_scale };

						if ((*particle_it).velocity.x == 0.0f) {
							std::uniform_real_distribution<float> dist(-3.0f, 3.0f);
							(*particle_it).velocity = {
								dist(random_engine_), dist(random_engine_), dist(random_engine_)
							};
						}
					}
				}

				// 行列計算
				Matrix4x4 scale_matrix = Math::MakeScaleMatrix((*particle_it).transform.scale);
				Matrix4x4 rotate_matrix = Math::MakeRotateMatrix((*particle_it).transform.rotate);
				Matrix4x4 translate_matrix = Math::MakeTranslateMatrix((*particle_it).transform.translate);
				Matrix4x4 world_matrix =
					Math::Multiply(scale_matrix,
						Math::Multiply(rotate_matrix,
							Math::Multiply(billboard_matrix, translate_matrix)));

				Matrix4x4 wvp_matrix = Math::Multiply(world_matrix, view_projection_matrix);

				group.instancing_data[group.num_instance].wvp = wvp_matrix;
				group.instancing_data[group.num_instance].world = world_matrix;

				// 速度反映
				(*particle_it).transform.translate.x += (*particle_it).velocity.x * kDeltaTime;
				(*particle_it).transform.translate.y += (*particle_it).velocity.y * kDeltaTime;
				(*particle_it).transform.translate.z += (*particle_it).velocity.z * kDeltaTime;

				group.instancing_data[group.num_instance].color.w = alpha;

				++group.num_instance;
			}

			++particle_it;
		}
	}
}
//描画
void ParticleManager::Draw() {
	// ルートシグネチャ
	directx_base_->GetCommandList()->SetGraphicsRootSignature(pso_->GetParticleRootSignature());
	// PSO 設定
	directx_base_->GetCommandList()->SetPipelineState(pso_->GetParticleGraphicsPipelineState());
	// プリミティブ設定
	directx_base_->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& [name, group] : particle_groups_) {
		// インスタンスがない場合はスキップ
		if (group.num_instance == 0) continue;

		// 頂点バッファ
		directx_base_->GetCommandList()->IASetVertexBuffers(0, 1, &group.vertex_buffer_view);

		// マテリアルCBuffer
		directx_base_->GetCommandList()->SetGraphicsRootConstantBufferView(0, material_resource_->GetGPUVirtualAddress());
		// テクスチャSRV
		directx_base_->GetCommandList()->SetGraphicsRootDescriptorTable(2, srv_manager_->GetGpuDescriptorHandle(group.material_data.textureIndex));
		// インスタンスSRV
		directx_base_->GetCommandList()->SetGraphicsRootDescriptorTable(1, srv_manager_->GetGpuDescriptorHandle(group.srv_index));

		// 描画
		directx_base_->GetCommandList()->DrawInstanced(UINT(group.model_data.vertices.size()), group.num_instance, 0, 0);
	}
}

// パーティクルグループ生成
void ParticleManager::CreateParticleGroup(const std::string name, const std::string texture_file_path, ParticleType type) {
	// 登録済みチェック
	assert(particle_groups_.find(name) == particle_groups_.end());

	ParticleGroup new_group;
	new_group.material_data.textureFilePath = texture_file_path;

	// テクスチャロード
	TextureManager::GetInstance()->LoadTexture(texture_file_path);
	new_group.material_data.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(texture_file_path);

	// 種類
	new_group.type = type;

	// 頂点生成
	if (model_data_.vertices.empty()) {
		if (type == ParticleType::Normal) VertexDataCreate(new_group.model_data);
		else if (type == ParticleType::Ring) RingVertexDataCreate(new_group.model_data);
		else if (type == ParticleType::Cylinder) CylinderVertexDataCreate(new_group.model_data);
		else if (type == ParticleType::Explosive) VertexDataCreate(new_group.model_data);
	}

	// 頂点バッファ生成
	new_group.vertex_resource = directx_base_->CreateBufferResource(sizeof(VertexData) * new_group.model_data.vertices.size());
	new_group.vertex_resource->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data_));
	std::memcpy(vertex_data_, new_group.model_data.vertices.data(), sizeof(VertexData) * new_group.model_data.vertices.size());

	new_group.vertex_buffer_view.BufferLocation = new_group.vertex_resource->GetGPUVirtualAddress();
	new_group.vertex_buffer_view.SizeInBytes = UINT(sizeof(VertexData) * new_group.model_data.vertices.size());
	new_group.vertex_buffer_view.StrideInBytes = sizeof(VertexData);

	// インスタンシング用リソース
	new_group.instancing_resource = directx_base_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	new_group.instancing_resource->Map(0, nullptr, reinterpret_cast<void**>(&new_group.instancing_data));

	for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
		new_group.instancing_data[i].wvp = Math::MakeIdentity4x4();
		new_group.instancing_data[i].world = Math::MakeIdentity4x4();
		new_group.instancing_data[i].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// SRV 割当
	new_group.srv_index = srv_manager_->Allocate();
	srv_manager_->CreateSrvForStructuredBuffer(new_group.srv_index, new_group.instancing_resource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));

	// 登録
	particle_groups_[name] = new_group;
}

// 新規パーティクル作成
ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& engine, const Vector3& translate, ParticleType type) {
	std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
	std::uniform_real_distribution<float> dist_color(0.0f, 1.0f);
	std::uniform_real_distribution<float> dist_time(1.0f, 3.0f);
	std::uniform_real_distribution<float> dist_scale(0.4f, 1.5f);
	std::uniform_real_distribution<float> dist_rotate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

	Particle particle;

	if (type == ParticleType::Normal) {
		particle.transform.scale = { 0.05f, dist_scale(engine), 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f, -10.0f, 0.0f };
		particle.color = { dist_color(engine), dist_color(engine), dist_color(engine) };
		particle.life_time = 1.0f;
	} else if (type == ParticleType::Ring) {
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f, 0.0f, 0.0f };
		particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		particle.life_time = 1.0f;
	} else if (type == ParticleType::Cylinder) {
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, 0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f, 0.0f, 0.0f };
		particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		particle.life_time = 99.0f; // 長寿命
	} else if (type == ParticleType::Explosive) {
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, -5.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f, 0.0f, 1.0f };
		particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		particle.life_time = 1.5f;
	}

	particle.current_time = 0.0f;
	return particle;
}
// パーティクル発生
void ParticleManager::Emit(const std::string& name, const Vector3& position, uint32_t count) {
	// 登録済みかチェック
	assert(particle_groups_.find(name) != particle_groups_.end());
	ParticleType type = particle_groups_[name].type;
	// 新たなパーティクル作成し、指定されたグループに登録
	for (uint32_t i = 0; i < count; ++i) {
		particle_groups_[name].particles.push_back(MakeNewParticle(random_engine_, position, type));
	}
}

// フィールド内判定
bool ParticleManager::IsCollision(const AABB& aabb, const Vector3& point) {
	if ((aabb.min.x <= point.x && aabb.max.x >= point.x) &&
		(aabb.min.y <= point.y && aabb.max.y >= point.y) &&
		(aabb.min.z <= point.z && aabb.max.z >= point.z)) {
		return true;
	}
	return false;
}

// 通常パーティクル頂点生成
void ParticleManager::VertexDataCreate(ModelData& modelData) {
	modelData.vertices.push_back({ { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } });
	modelData.vertices.push_back({ { -1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } });
	modelData.vertices.push_back({ { 1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } });
	modelData.vertices.push_back({ { 1.0f, -1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } });
	modelData.vertices.push_back({ { -1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } });
	modelData.vertices.push_back({ { -1.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } });

	modelData.material.textureFilePath = "./resources/circle.png";

	CreateAndMapVertexBuffer(directx_base_,modelData.vertices,vertex_resource_,vertex_buffer_view_,vertex_data_);
}
// Ring型頂点生成
void ParticleManager::RingVertexDataCreate(ModelData& modelData) {
	const uint32_t kRingDivide = 32;
	const float kOuterRadius = 1.0f;
	const float kInnerRadius = 0.2f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kRingDivide);

	for (uint32_t index = 0; index < kRingDivide; ++index) {
		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1) * radianPerDivide);
		float cosNext = std::cos((index + 1) * radianPerDivide);
		float u = float(index) / float(kRingDivide);
		float uNext = float(index + 1) / float(kRingDivide);

		modelData.vertices.push_back({ { -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f }, { u, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f } });

		modelData.vertices.push_back({ { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f }, { uNext, 1.0f }, { 0.0f, 0.0f, 1.0f } });
	}

	modelData.material.textureFilePath = "./resources/gradationLine.png";

	CreateAndMapVertexBuffer(directx_base_,modelData.vertices,vertex_resource_,vertex_buffer_view_,vertex_data_);
}

// Cylinder型パーティクルの頂点データ作成
void ParticleManager::CylinderVertexDataCreate(ModelData& modelData) {
	const uint32_t kCylinderDivide = 32;       // 円周分割数
	const float kTopRadius = 1.0f;             // 上面半径
	const float kBottomRadius = 1.0f;          // 底面半径
	const float kHeight = 3.0f;                // 高さ
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kCylinderDivide); // 1分割あたりのラジアン

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {
		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1) * radianPerDivide);
		float cosNext = std::cos((index + 1) * radianPerDivide);
		float u = float(index) / float(kCylinderDivide);
		float uNext = float(index + 1) / float(kCylinderDivide);

		// 上面三角形
		modelData.vertices.push_back({ { -sin * kTopRadius, kHeight, cos * kTopRadius, 1.0f }, { u, 0.0f }, { -sin, 0.0f, cos } });
		modelData.vertices.push_back({ { -sinNext * kTopRadius, kHeight, cosNext * kTopRadius, 1.0f }, { uNext, 0.0f }, { -sinNext, 0.0f, cosNext } });
		modelData.vertices.push_back({ { -sin * kBottomRadius, 0.0f, cos * kBottomRadius, 1.0f }, { u, 1.0f }, { -sin, 0.0f, cos } });

		// 側面の三角形
		modelData.vertices.push_back({ { -sin * kBottomRadius, 0.0f, cos * kBottomRadius, 1.0f }, { u, 1.0f }, { -sin, 0.0f, cos } });
		modelData.vertices.push_back({ { -sinNext * kTopRadius, kHeight, cosNext * kTopRadius, 1.0f }, { uNext, 0.0f }, { -sinNext, 0.0f, cosNext } });
		modelData.vertices.push_back({ { -sinNext * kBottomRadius, 0.0f, cosNext * kBottomRadius, 1.0f }, { uNext, 1.0f }, { -sinNext, 0.0f, cosNext } });
	}

	CreateAndMapVertexBuffer(directx_base_,modelData.vertices,vertex_resource_,vertex_buffer_view_,vertex_data_);
}
// マテリアル用GPUバッファ生成と初期化
void ParticleManager::MaterialCreate() {
	// GPUバッファ生成
	material_resource_ = directx_base_->CreateBufferResource(sizeof(Material));

	// 書き込み用アドレス取得
	material_resource_->Map(0, nullptr, reinterpret_cast<void**>(&material_data_));

	// マテリアル初期値設定
	material_data_->color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };       // デフォルト色
	material_data_->enableLighting = false;                           // ライティング無効
	material_data_->uvTransform = Math::MakeIdentity4x4();            // UV変換行列は単位行列
}

// 頂点バッファ生成とマッピングを共通化
void ParticleManager::CreateAndMapVertexBuffer(
	DirectXBase* directx_base,
	const std::vector<VertexData>& vertices,
	Microsoft::WRL::ComPtr<ID3D12Resource>& vertex_resource,
	D3D12_VERTEX_BUFFER_VIEW& vertex_buffer_view,
	VertexData*& vertex_data)
{
    // 頂点バッファ用のGPUリソースを生成
    vertex_resource = directx_base->CreateBufferResource(sizeof(VertexData) * vertices.size());

    // バッファビュー情報を設定
    vertex_buffer_view.BufferLocation = vertex_resource->GetGPUVirtualAddress();
    vertex_buffer_view.SizeInBytes = UINT(sizeof(VertexData) * vertices.size());
    vertex_buffer_view.StrideInBytes = sizeof(VertexData);

    // GPUリソースをCPUからアクセスできるようにマッピングし、データを書き込む
    vertex_resource->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data));
    std::memcpy(vertex_data, vertices.data(), sizeof(VertexData) * vertices.size());
}

// 指定グループ名のパーティクルタイプ取得
ParticleType ParticleManager::GetParticleType(const std::string& name) {
	// 登録済みチェック
	assert(particle_groups_.find(name) != particle_groups_.end());
	return particle_groups_[name].type;
}