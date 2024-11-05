#include "Object3d.h"
#include "Object3dBase.h"
#include <fstream>
#include "Math.h"
#include "TextureManager.h"

//初期化
void Object3d::Initialize(Object3dBase* object3dBase) {
	this->object3dBase_ = object3dBase;
	//モデル読み込み
	modelData_ = LoadObjFile("resources", "plane.obj");
	VertexDataCreate();
	MaterialCreate();
	TransformationCreate();
	DirectionalLightCreate();
	//.objの参照しているテクスチャファイル読み込み
	TextureManager::GetInstance()->LoadTexture(modelData_.material.textureFilePath);
	//読み込んだテクスチャの番号を取得
	modelData_.material.textureIndex =
		TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData_.material.textureFilePath);
	//Transform変数を作る
	transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform_ = { {1.0f,1.0f,1.0f},{0.3f,0.0f,0.0f},{0.0f,4.0f,-10.0f} };
}
//更新
void Object3d::Update() {
	//TransformからWorldMatrixを作る
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	//ViewMatrixを作って単位行列を代入
	Matrix4x4 cameraMatrix = Math::MakeAffineMatrix(cameraTransform_.scale, cameraTransform_.rotate, cameraTransform_.translate);
	Matrix4x4 viewMatrix = Math::Inverse(cameraMatrix);
	//ProjectionMatrixを作って平行投影行列を書き込む
	Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, float(WindowsAPI::kClientWitdh) / float(WindowsAPI::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix= Math::Multiply(worldMatrix, Math::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}
//描画
void Object3d::Draw() {
	//VertexBufferViewを設定
	object3dBase_->GetDxBase()->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//マテリアルCBufferの場所を設定
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//座標変換行列CBufferの場所を設定
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(modelData_.material.textureIndex));
	//Lighting
	object3dBase_->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(3, DirectionalLightResource->GetGPUVirtualAddress());
	//DrawCall(描画)
	object3dBase_->GetDxBase()->Getcommandlist()->DrawInstanced(UINT(modelData_.vertices.size()), 1,0, 0);
}
//頂点データ作成
void Object3d::VertexDataCreate() {
	//リソースを作る
	vertexResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
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
void Object3d::MaterialCreate() {
	//リソースを作る
	materialResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(Material));
	// 書き込むためのアドレスと取得
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//マテリアルデータの初期値を書き込む
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
	//Lighting
	materialData->enableLighting = false;
	//UVTransform行列を単位行列で初期化
	materialData->uvTransform = Math::MakeIdentity4x4();
}
//座標変換行列データ作成
void Object3d::TransformationCreate() {
	//リソースを作る
	transformationMatrixResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスの取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}
//平行光源データ作成
void Object3d::DirectionalLightCreate() {
	DirectionalLightResource = object3dBase_->GetDxBase()->CreateBufferResource(sizeof(DirectionalLight));
	//書き込むためのアドレスと取得
	DirectionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLight));
	//デフォルト値はとりあえず以下のようにしておく
	directionalLight->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLight->direction = { 0.0f,-1.0f,0.0f };
	directionalLight->intensity = 1.0f;
}
//.mtlファイルの読み取り
Object3d::MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename) {
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
Object3d::ModelData Object3d::LoadObjFile(const std::string& directoryPath, const std::string& filename) {
	//1.中で必要となる変数の宣言
	ModelData modelData;//構築するModelData
	std::vector<Vector4>positions;//位置
	std::vector<Vector3>normals;//法線
	std::vector<Vector2>texcoords;//テクスチャ座標
	std::string line;//ファイルから読んだ1行を格納するもの

	//2.ファイルを開く
	std::ifstream file(directoryPath + "/" + filename);//ファイルを開く
	assert(file.is_open());//開けなかったら止める

	//3.実際にファイルを読み、ModelDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;//先頭の識別子を読む
		//identifierに応じた処理
		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			position.x *= -1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normal.x *= -1.0f;
			normals.push_back(normal);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
			//面は三角形限定。その他は未対応
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				//頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');//区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				//要素へのIndexから、実際の要素の値を取得して、頂点を構築
				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position,texcoord,normal };
				modelData.vertices.push_back(vertex);

				triangle[faceVertex] = { position,texcoord,normal };
			}
			//頂点を逆順で登録することで、周り順を逆にする
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
		else if (identifier == "mtllib") {
			//materialTemplateLibraryファイルの名前を取得する
			std::string materialFilename;
			s >> materialFilename;
			//基本的にobjファイルと同一階層にmtlは存在させるので、ディレクトリ名とファイル名を渡す
			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	//4.ModelDataを返す
	return modelData;
}