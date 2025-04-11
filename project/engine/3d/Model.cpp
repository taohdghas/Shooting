#include "Model.h"
#include "TextureManager.h"
#include "Math.h"
#include <cassert>
#include <fstream>
#include <sstream>

//初期化
void Model::Initialize(ModelBase* modelBase, const std::string& directorypath, const std::string& filename) {
	modelBase_ = modelBase;
	modelData_ = LoadModelFile(directorypath, filename);
	VertexDataCreate();
	MaterialCreate();
	//.objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	//読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex =
		TextureManager::GetInstance()->GetSrvIndex(modelData_.material.textureFilePath);
}
//描画
void Model::Draw() {
	//VertexBufferViewを設定
	modelBase_->GetDxBase()->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//マテリアルCBufferの場所を設定
	modelBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	modelBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureFilePath));

	//DrawCall(描画)
	modelBase_->GetDxBase()->Getcommandlist()->DrawInstanced(UINT(modelData_.vertices.size()), 1, 0, 0);
}
//頂点データ作成
void Model::VertexDataCreate() {
	//リソースを作る
	vertexResource = modelBase_->GetDxBase()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点のサイズ
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
	// 1頂点あたりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// 頂点リソースにデータを書き込む
	// 書き込むためのアドレスを取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//頂点データをリソースにコピー
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
}
//マテリアル作成
void Model::MaterialCreate() {
	//リソースを作る
	materialResource = modelBase_->GetDxBase()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスと取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//マテリアルデータの初期値を書き込む
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	//Lighting
	materialData->enableLighting = true;
	//UVTransform行列を単位行列で初期化
	materialData->uvTransform = Math::MakeIdentity4x4();
	materialData->shininess = 40.0f;

}
//.mtlファイルの読み取り
MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	MaterialData materialData;//構築するMaterialData
	std::string line;//ファイルから読んだ1行を格納するもの
	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//とりあえず開けなかったら止める
	//3.実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		//identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			//連結してファイルパスにする
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	//4.MaterialDataを返す
	return materialData;
}
//.objファイルの読み取り
ModelData Model::LoadModelFile(const std::string& directoryPath, const std::string& filename) {

	ModelData modelData;//構築するModelData
	//assimpのSceneを構築
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	//Scene全体の階層構造構築
	modelData.rootNode = ReadNode(scene->mRootNode);
	assert(scene->HasMeshes());//メッシュがない場合対応しない

	//Meshの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());//法線がないMeshは非対応
		assert(mesh->HasTextureCoords(0));//TexcoordがないMeshは非対応
		//faceの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);//三角形のみ
			//Vertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				VertexData vertex;
				vertex.position = { position.x,position.y,position.z,1.0f };
				vertex.normal = { normal.x,normal.y,normal.z };
				vertex.texcoord = { texcoord.x,texcoord.y };
				//aiProcess_MakeLeftHandedはz*=-1で右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}
	}

	//materialの解析
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

//assimpの構造体変換
Node Model::ReadNode(aiNode* node) {
	Node result;
	aiMatrix4x4 aiLocalMatrix = node->mTransformation;//nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose();//列ベクトル形式を行ベクトル形式に転置
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j)
		{
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}
	result.name = node->mName.C_Str();//Node名を格納
	result.children.resize(node->mNumChildren);//子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		//再帰的に読んで階層構造を作っていく
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}

//animationファイル読み込み
Animation Model::LoadAnimationFile(const std::string& directoryPath, const std::string& filename) {
	Animation animation;
	Assimp::Importer importer;
	std::string filePath = directoryPath + "/" + filename;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), 0);
	assert(scene->mNumAnimations != 0);//アニメーションがない時
	aiAnimation* animationAssimp = scene->mAnimations[0];//最初のアニメーションのみ採用
	//時間の単位を秒に変換
	animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);
	//channelを回してNodeAnimationの情報をとってくる
	for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; ++channelIndex) {
		aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
		NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
			KeyframeVector3 keyframe;
			//秒に変換
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			//右手->左手
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.translate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; ++keyIndex) {
			aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
			KeyframeQuaternion keyframe;
			//秒に変換
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
		    //右手->左手
			keyframe.value = { keyAssimp.mValue.x,-keyAssimp.mValue.y,-keyAssimp.mValue.z,keyAssimp.mValue.w};
			nodeAnimation.rotate.keyframes.push_back(keyframe);
		}
		for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; ++keyIndex) {
			aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
			KeyframeVector3 keyframe;
			//秒に変換
			keyframe.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
			//右手->左手
			keyframe.value = { -keyAssimp.mValue.x,keyAssimp.mValue.y,keyAssimp.mValue.z };
			nodeAnimation.scale.keyframes.push_back(keyframe);
		}
	}
	return animation;
}
		/*
		
		
		
		*/