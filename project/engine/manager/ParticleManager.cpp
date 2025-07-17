#include "ParticleManager.h"
#include "MyMath.h"
#include "CameraManager.h"
#include <assert.h>
#include <numbers>

ParticleManager* ParticleManager::instance = nullptr;
//シングルトンインスタンスの取得
ParticleManager* ParticleManager::GetInstance() {
	if (instance == nullptr) {
		instance = new ParticleManager;
	}
	return instance;
}

void ParticleManager::Initialize(DirectXBase* directxBase, SrvManager* srvManager, Camera* camera) {
	this->directxBase_ = directxBase;
	this->srvManager_ = srvManager;
	pso_ = std::make_unique<Pso>();
	pso_->Initialize(directxBase_);
	this->camera_ = camera;
	//乱数生成器
	randomEngine.seed(seedGenerator());
	//グラフィックスパイプライン
	pso_->CreateParticlePipelineState();
	//マテリアルデータ
	MaterialCreate();
	//Field
	accelerationfield_.acceleration = { 15.0f,0.0f,0.0f };
	accelerationfield_.area.min = { -1.0f,-1.0f,-1.0f };
	accelerationfield_.area.max = { 1.0f,1.0f,1.0f };
}

//終了
void ParticleManager::Finalize() {
	delete instance;
	instance = nullptr;
	//particleGroups.clear();
}

//シーン終了時に呼ぶ
void ParticleManager::Clear() {
	particleGroups.clear();
}

void ParticleManager::Update() {
	Matrix4x4 cameraMatrix =CameraManager::GetInstance()->GetActiveCamera()->GetWorldMatrix();
	Matrix4x4 backToFrontMatrix = Math::MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billboardMatrix = Math::Multiply(backToFrontMatrix, cameraMatrix);
	//ビュー行列
	Matrix4x4 viewMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewMatrix();
	//プロジェクション行列
	Matrix4x4 projectionMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix();
	//ビュープロジェクション行列
	Matrix4x4 viewprojectionMatrix = CameraManager::GetInstance()->GetActiveCamera()->GetViewProjectionMatrix();
	//trueなら使う
	if (useBillboard) {
		billboardMatrix.m[3][0] = 0.0f;
		billboardMatrix.m[3][1] = 0.0f;
		billboardMatrix.m[3][2] = 0.0f;
	}
	//falseなら単位行列
	else if (!useBillboard) {
		billboardMatrix = Math::MakeIdentity4x4();
	}
	for (auto& ParticleGroups : particleGroups) {
		ParticleGroups.second.kNumInstance = 0;
		for (std::list<Particle>::iterator particleIterator = ParticleGroups.second.particles.begin();
			particleIterator != ParticleGroups.second.particles.end();) {
			//時間を更新
			(*particleIterator).currentTime += kDeltaTime;
			//寿命に達したらグループから外す
			if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
				particleIterator = ParticleGroups.second.particles.erase(particleIterator);
				continue;
			}
			float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
			if (ParticleGroups.second.kNumInstance < kNumMaxInstance) {

				//Cylinder回転
				if (ParticleGroups.second.type == ParticleType::Cylinder) {
					(*particleIterator).transform.rotate.y += 1.0f * kDeltaTime;
				}

				if (ParticleGroups.second.type == ParticleType::Explosive) {
					float t = (*particleIterator).currentTime / (*particleIterator).lifeTime;

					if (t < 0.5f) {
						//縮小
						float scale = 1.0f - t * 2.0f;
						scale = max(scale, 0.05f);
						(*particleIterator).transform.scale = { scale, scale, scale };
					} else {
						//爆発
						std::uniform_real_distribution<float> distScale(0.1f, 0.35f);
						float randomScale = distScale(randomEngine);
						(*particleIterator).transform.scale = { randomScale, randomScale, randomScale };
						//拡散
						if ((*particleIterator).velocity.x == 0.0f) {
							std::uniform_real_distribution<float> dist(-3.0f, 3.0f);
							(*particleIterator).velocity = {
								dist(randomEngine), dist(randomEngine), dist(randomEngine)
							};
						}
					}
				}

				//行列計算
				Matrix4x4 scaleMatrix = Math::MakeScaleMatrix((*particleIterator).transform.scale);
				Matrix4x4 rotateMatrix = Math::MakeRotateMatrix((*particleIterator).transform.rotate);
				Matrix4x4 translateMatrix = Math::MakeTranslateMatrix((*particleIterator).transform.translate);
				Matrix4x4 worldMatrix = Math::Multiply(scaleMatrix, Math::Multiply(rotateMatrix, Math::Multiply(billboardMatrix, translateMatrix)));
				Matrix4x4 worldViewProjectionMatrix = Math::Multiply(worldMatrix, viewprojectionMatrix);
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].WVP = worldViewProjectionMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].World = worldMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].color;
				//Fieldの範囲内のParticleには加速度を適用する

				//速度を適用
				(*particleIterator).transform.translate.x += (*particleIterator).velocity.x * kDeltaTime;
				(*particleIterator).transform.translate.y += (*particleIterator).velocity.y * kDeltaTime;
				(*particleIterator).transform.translate.z += (*particleIterator).velocity.z * kDeltaTime;
				//インスタンシング用データ書き込み
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].WVP = worldViewProjectionMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].World = worldMatrix;
				ParticleGroups.second.instancingData[ParticleGroups.second.kNumInstance].color.w = alpha;
				++ParticleGroups.second.kNumInstance;
			}
			++particleIterator;
		}
	}
}

void ParticleManager::Draw() {

	//ルートシグネチャ
	directxBase_->Getcommandlist()->SetGraphicsRootSignature(pso_->GetParticleRootSignature());
	//PSO設定
	directxBase_->Getcommandlist()->SetPipelineState(pso_->GetParticleGraphicsPipelineState());
	//描画形状設定
	directxBase_->Getcommandlist()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//パーティクルについて処理
	for (auto& [name, ParticleGroup] : particleGroups) {
		//頂点バッファ切り替え
		directxBase_->Getcommandlist()->IASetVertexBuffers(0, 1, &ParticleGroup.vertexBufferView);

		//マテリアルCBufferの場所設定
		directxBase_->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
		//テクスチャSRVセット
		directxBase_->Getcommandlist()->SetGraphicsRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(ParticleGroup.materialData.textureIndex));
		//インスタンスSRVセット
		directxBase_->Getcommandlist()->SetGraphicsRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(ParticleGroup.SRVIndex));
		//描画
		directxBase_->Getcommandlist()->DrawInstanced(UINT(ParticleGroup.modelData.vertices.size()), ParticleGroup.kNumInstance, 0, 0);
	}
}

//パーティクルグループの生成
void ParticleManager::CreateparticleGroup(const std::string name, const std::string textureFilePath, ParticleType type) {
	//登録済みの名前かチェック
	assert(particleGroups.find(name) == particleGroups.end());
	ParticleGroup newParticle;
	//particleGroups[name] = newParticle;
	//テクスチャファイルパスを設定
	newParticle.materialData.textureFilePath = textureFilePath;
	//テクスチャを読み込む
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	//マテリアルデータにテクスチャSRVインデックスを記録
	newParticle.materialData.textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
	//パーティクルの種類
	newParticle.type = type;

	//頂点生成

	if (modelData.vertices.empty()) {
		if (type == ParticleType::Normal) {
			VertexDataCreate(newParticle.modelData);
		} else if (type == ParticleType::Ring) {
			RingVertexDataCreate(newParticle.modelData);
		} else if (type == ParticleType::Cylinder) {
			CylinderVertexDataCreate(newParticle.modelData);
		} else if (type == ParticleType::Explosive) {
			VertexDataCreate(newParticle.modelData);
		}
	}

	newParticle.vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * newParticle.modelData.vertices.size());
	newParticle.vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, newParticle.modelData.vertices.data(), sizeof(VertexData) * newParticle.modelData.vertices.size());

	newParticle.vertexBufferView.BufferLocation = newParticle.vertexResource->GetGPUVirtualAddress();
	newParticle.vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * newParticle.modelData.vertices.size());
	newParticle.vertexBufferView.StrideInBytes = sizeof(VertexData);

	//インスタンシング用のリソース生成
	newParticle.instancingResource = directxBase_->CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
	//アドレス取得
	newParticle.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newParticle.instancingData));
	for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
		newParticle.instancingData[index].WVP = Math::MakeIdentity4x4();
		newParticle.instancingData[index].World = Math::MakeIdentity4x4();
		newParticle.instancingData[index].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//SRV生成
	newParticle.SRVIndex = srvManager_->Allccate();
	srvManager_->CreateSRVforStructuredBuffer(newParticle.SRVIndex, newParticle.
		instancingResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));

	particleGroups[name] = newParticle;
}

//パーティクル生成関数
ParticleManager::Particle ParticleManager::MakeNewParticle(std::mt19937& randomEngine, const Vector3& translate, ParticleType type) {
	std::uniform_real_distribution<float>distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float>distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float>distTime(1.0f, 3.0f);
	std::uniform_real_distribution<float>distScale(0.4f, 1.5f);
	std::uniform_real_distribution<float>distRotate(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);
	Vector3 randomTranslate{ distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	Particle particle;

	//通常
	if (type == ParticleType::Normal) {
		particle.transform.scale = { 0.05f,distScale(randomEngine),1.0f };
		particle.transform.rotate = { 0.0f,0.0f,0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f,-10.0f,0.0f };
		particle.color = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine) };
		particle.lifeTime = 1.0f;

	} else if (type == ParticleType::Ring) {
		//Ring型
		particle.transform.scale = { 1.0f,1.0f,1.0f };
		particle.transform.rotate = { 0.0f,0.0f,0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f,0.0f,0.0f };
		particle.color = { 1.0f,1.0f,1.0f,1.0f };
		particle.lifeTime = 1.0f;

	} else if (type == ParticleType::Cylinder) {
		//Cylinder型
		particle.transform.scale = { 1.0f,1.0f,1.0f };
		particle.transform.rotate = { 0.0f,0.0f,0.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f,0.0f,0.0f };
		particle.color = { 1.0f,1.0f,1.0f,1.0f };
		particle.lifeTime = 99.0f;
	} else if (type == ParticleType::Explosive) {
		particle.transform.scale = { 1.0f, 1.0f, 1.0f };
		particle.transform.rotate = { 0.0f, 0.0f, -5.0f };
		particle.transform.translate = translate;
		particle.velocity = { 0.0f, 0.0f, 1.0f };
		particle.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		particle.lifeTime = 1.5f;
	}
	particle.currentTime = 0.0f;
	return particle;
}

//パーティクルの発生
void ParticleManager::Emit(const std::string name, const Vector3& position, uint32_t count) {
	//登録済みかチェック
	assert(particleGroups.find(name) != particleGroups.end());
	ParticleType type = particleGroups[name].type;
	//新たなパーティクル作成し、指定されたグループに登録
	for (uint32_t i = 0; i < count; ++i) {
		particleGroups[name].particles.push_back(MakeNewParticle(randomEngine, position, type));
	}
}

//ParticleがFieldの範囲内か判定
bool ParticleManager::IsCollision(const AABB& aabb, const Vector3& point) {
	if ((aabb_.min.x <= point.x && aabb_.max.x >= point.x) &&
		(aabb_.min.y <= point.y && aabb.max.y >= point.y) &&
		(aabb.min.z <= point.z && aabb.max.z >= point.z)) {
		return true;
	} else {
		return false;
	}
}

//頂点データ作成
void ParticleManager::VertexDataCreate(ModelData& modelData) {
	modelData.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f},.texcoord = {0.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//左下
	modelData.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f},.texcoord = {1.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//右上
	modelData.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f},.texcoord = {1.0f,1.0f},.normal = {0.0f,0.0f,1.0f} });//右下
	modelData.material.textureFilePath = "./resources/circle.png";
	//リソースを作る
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}
//Ringの頂点データ生成
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
		//positionとuv(必要ならnormalのzも)

		modelData.vertices.push_back({ { -sin * kOuterRadius, cos * kOuterRadius, 0.0f, 1.0f }, { u, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f } });

		modelData.vertices.push_back({ { -sin * kInnerRadius, cos * kInnerRadius, 0.0f, 1.0f }, { u, 1.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kOuterRadius, cosNext * kOuterRadius, 0.0f, 1.0f }, { uNext, 0.0f }, { 0.0f, 0.0f, 1.0f } });
		modelData.vertices.push_back({ { -sinNext * kInnerRadius, cosNext * kInnerRadius, 0.0f, 1.0f }, { uNext, 1.0f }, { 0.0f, 0.0f, 1.0f } });
	}
	modelData.material.textureFilePath = "./resources/gradationLine.png";
	//リソースを作る
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込む為のアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());
}

//Cylinderの頂点データ作成
void ParticleManager::CylinderVertexDataCreate(ModelData& modelData) {
	const uint32_t kCylinderDivide = 32;
	const float kTopRadius = 1.0f;
	const float kBottomRadius = 1.0f;
	const float kHeight = 3.0f;
	const float radianPerDivide = 2.0f * std::numbers::pi_v<float> / float(kCylinderDivide);

	for (uint32_t index = 0; index < kCylinderDivide; ++index) {
		float sin = std::sin(index * radianPerDivide);
		float cos = std::cos(index * radianPerDivide);
		float sinNext = std::sin((index + 1) * radianPerDivide);
		float cosNext = std::cos((index + 1) * radianPerDivide);
		float u = float(index) / float(kCylinderDivide);
		float uNext = float(index + 1) / float(kCylinderDivide);

		//positoin,texcoord,normal
		modelData.vertices.push_back({ { -sin * kTopRadius,kHeight,cos * kTopRadius,1.0f }, { u, 0.0f }, {-sin, 0.0f,cos } });
		modelData.vertices.push_back({ { -sinNext * kTopRadius,kHeight, cosNext * kTopRadius, 1.0f }, { uNext, 0.0f }, { -sinNext, 0.0f,cosNext } });
		modelData.vertices.push_back({ { -sin * kBottomRadius,0.0f, cos * kBottomRadius, 1.0f }, { u, 1.0f }, { -sin, 0.0f,cos } });

		modelData.vertices.push_back({ { -sin * kBottomRadius,0.0f, cos * kBottomRadius, 1.0f }, { u, 1.0f }, { -sin, 0.0f,cos} });
		modelData.vertices.push_back({ { -sinNext * kTopRadius,kHeight, cosNext * kTopRadius, 1.0f }, { uNext, 0.0f }, {-sinNext, 0.0f,cosNext } });
		modelData.vertices.push_back({ { -sinNext * kBottomRadius, 0.0f,cosNext * kBottomRadius,1.0f }, { uNext, 1.0f }, {-sinNext, 0.0f,cosNext } });
	}
	//リソースを作る
	vertexResource = directxBase_->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	//リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	//1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込む為のアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

}

//マテリアルデータ作成
void ParticleManager::MaterialCreate() {
	//リソースを作るdousiyou int GAkuHatutemia;
	materialResource = directxBase_->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスと取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//マテリアルデータの初期値を書き込む
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	//Lighting
	materialData->enableLighting = false;
	//UVTransform行列を単位行列で初期化
	materialData->uvTransform = Math::MakeIdentity4x4();
}

//ParticleTypeのゲッター
ParticleType ParticleManager::GetParticleType(const std::string& name) {
	assert(particleGroups.find(name) != particleGroups.end());
	return particleGroups[name].type;
}