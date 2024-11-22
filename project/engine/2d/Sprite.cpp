#include "Sprite.h"
#include "SpriteBase.h"
#include  "TextureManager.h"
#include "Math.h"
#include "Transform.h"

//初期化
void Sprite::Initialize(SpriteBase* spriteBase, std::string textureFilePath) {
	this->spriteBase = spriteBase;
	VertexDataCreate();
	IndexCreate();
	MaterialCreate();
	TransformationCreate();
	AdjustTextureSize();

	//textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
	filePath = textureFilePath;
}
//更新
void Sprite::Update() {
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	//左右反転
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	const DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetaData(filePath);
	float tex_left = textureLeftTop.x / metadata.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
	float tex_top = textureLeftTop.y / metadata.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

	// 頂点リソースにデータを書き込む
	vertexData[0].position = { left,bottom,0.0f,1.0f };// 左下
	vertexData[0].texcoord = { tex_left,tex_bottom };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };
	vertexData[1].position = { left,top,0.0f,1.0f };// 左上
	vertexData[1].texcoord = { tex_left,tex_top };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };
	vertexData[2].position = { right,bottom,0.0f,1.0f };// 右下
	vertexData[2].texcoord = { tex_right,tex_bottom};
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };
	vertexData[3].position = { right,top,0.0f,1.0f };// 左上
	vertexData[3].texcoord = { tex_right,tex_top };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };
	/*
	vertexData[4].position = { 640.0f,0.0f,0.0f,1.0f };// 右上
	vertexData[4].texcoord = { 1.0f,0.0f };
	vertexData[4].normal = { 0.0f,0.0f,-1.0f };
	vertexData[5].position = { 640.0f,360.0f,0.0f,1.0f };// 右下
	vertexData[5].texcoord = { 1.0f,1.0f };
	vertexData[5].normal = { 0.0f,0.0f,-1.0f };
	*/

	//Transform情報を作る
	Transform transform{ {size.x,size.y,1.0f},{0.0f,0.0f,rotation},{position.x,position.y,0.0f} };
	//TransformからWorldMatrixを作る
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	//ViewMatrixを作って単位行列を代入
	Matrix4x4 viewMatrix = Math::MakeIdentity4x4();
	//ProjectionMatrixを作って平行投影行列を書き込む
	Matrix4x4 projectionMatrix = Math::MakeOrthographicMatrix(0.0f, 0.0f, float(WindowsAPI::kClientWitdh), float(WindowsAPI::kClientHeight), 0.0f, 100.0f);
	//	Matrix4x4 worldViewProjectionMatrixSprite = Math::Multiply(worldMatrix, Math::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = Math::Multiply(worldMatrix, Math::Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->World = worldMatrix;
}
//描画
void Sprite::Draw() {
//	textureSrvHandleGPU = spriteBase->GetDxBase()->GetSRVGPUDescriptorHandle(1);
	//VertexBufferViewを設定
	spriteBase->GetDxBase()->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
	//IndexBufferViewを設定
	spriteBase->GetDxBase()->Getcommandlist()->IASetIndexBuffer(&indexBufferView);
	//マテリアルCBufferの場所を設定
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	//座標変換行列CBufferの場所を設定
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	//SRVのDescriptorTableの先頭を設定
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(2,TextureManager::GetInstance()->GetSrvHandleGPU(filePath));
	//DrawCall(描画)
	spriteBase->GetDxBase()->Getcommandlist()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}
/*
//テクスチャ変更
void Sprite::SetTexture(std::string textureFilePath) {
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(textureFilePath);
}
*/
//頂点データ作成
void Sprite::VertexDataCreate() {
	//リソースを作る
	vertexResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(VertexData) * 4);
	// リソースの先頭のアドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点6つ分
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	// 1頂点当たりのサイズ
	vertexBufferView.StrideInBytes = sizeof(VertexData);
	//VertexResourceにデータを書き込むためのアドレス取得
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}
//index作成
void Sprite::IndexCreate() {
	//リソースを作る
	indexResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(uint32_t) * 6);
	//リソースの先頭のアドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	//IndexResourceにデータを書き込むためのアドレスを取得
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	//インデックスリソースにデータを書き込む
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;
}
//マテリアル作成
void Sprite::MaterialCreate() {
	//リソースを作る
	materialResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(Material));
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
void Sprite::TransformationCreate() {
	//リソースを作る
	transformationMatrixResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));
	//書き込むためのアドレスの取得
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	// 単位行列を書き込んでおく
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}
//テクスチャサイズをイメージに合わせる
void Sprite::AdjustTextureSize() {
	//テクスチャメタデータを取得
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(filePath);

	textureSize.x = static_cast<float>(metadata.width);
	textureSize.y = static_cast<float>(metadata.height);
	//画像サイズをテクスチャサイズに合わせる
	size = textureSize;
}