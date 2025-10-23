#include "Model.h"
#include "TextureManager.h"
#include "MyMath.h"
#include <cassert>
#include <fstream>
#include <sstream>

// 初期化
void Model::Initialize(ModelBase* modelBase, const std::string& directorypath, const std::string& filename) {
	modelBase_ = modelBase;

	// .objファイルを読み込み、モデルデータを構築
	modelData_ = LoadModelFile(directorypath, filename);

	// 頂点データとマテリアルデータをGPUリソースとして生成
	VertexDataCreate();
	MaterialCreate();

	// .objが参照しているテクスチャファイルを読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);

	// 読み込んだテクスチャのSRVインデックスを取得してマテリアルに設定
	modelData_.material.textureIndex =
		TextureManager::GetInstance()->GetSrvIndex(modelData_.material.textureFilePath);
}

// 描画
void Model::Draw() {
	// 頂点バッファをコマンドリストに設定
	modelBase_->GetDxBase()->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);

	// マテリアルの定数バッファを設定（ルートパラメータ0）
	modelBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());

	// テクスチャSRVのディスクリプタテーブルを設定（ルートパラメータ2）
	modelBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));

	// DrawCall発行（頂点数分のインスタンスを描画）
	modelBase_->GetDxBase()->Getcommandlist()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}

// 頂点データ作成
void Model::VertexDataCreate() {
	// 頂点バッファ用のGPUリソースを生成
	vertexResource = modelBase_->GetDxBase()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());

	// バッファビュー情報を設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// GPUリソースへ頂点データを書き込む
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}

// マテリアル作成
void Model::MaterialCreate() {
	// マテリアル用の定数バッファリソースを生成
	materialResource = modelBase_->GetDxBase()->CreateBufferResource(sizeof(Material));

	// 書き込み先アドレスを取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	// マテリアルの初期設定
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	materialData->enableLighting = true; // ライティングを有効化
	materialData->uvTransform = Math::MakeIdentity4x4(); // UV行列を単位行列で初期化
	materialData->shininess = 40.0f; // 反射の強さを設定
}

// .mtlファイルの読み取り
MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	MaterialData materialData;
	std::string line;

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	// ファイルを1行ずつ読み込み、マテリアル情報を解析
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// テクスチャファイルの指定を検出した場合
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// テクスチャファイルへのフルパスを構築
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}

// .objファイルの読み取り
ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {
	ModelData modelData;

	// Assimpを使用して.objを読み込む
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);

	// 階層構造（ノード）の読み込み
	modelData.rootNode = ReadNode(scene->mRootNode);
	assert(scene->HasMeshes());

	// Meshデータの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());
		assert(mesh->HasTextureCoords(0));

		// 各フェイス（三角形）を解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);

			// フェイス内の各頂点を解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				// 頂点データ構築
				VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };

				// 左手座標系へ変換（Assimpが右手→左手変換を行うため補正）
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				modelData.vertices.push_back(vertex);
			}
		}
	}

	// マテリアル情報の解析（テクスチャパスの取得）
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
		}
	}

	return modelData;
}

// assimpのノード構造体を独自Nodeに変換
Node Model::ReadNode(aiNode* node) {
	Node result;

	// ノードのローカル行列を取得し、行列形式を転置（列ベクトル→行ベクトル）
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();

	// 行列データをコピー
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}

	// ノード名と子ノードを設定
	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);

	// 子ノードを再帰的に読み込み、階層構造を構築
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
}
