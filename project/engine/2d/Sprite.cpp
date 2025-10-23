#include "Sprite.h"
#include "SpriteBase.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "Transform.h"

//初期化
void Sprite::Initialize(SpriteBase* spriteBase, std::string textureFilePath) {
	this->spriteBase = spriteBase;

	// 頂点バッファの生成と設定
	VertexDataCreate();
	// インデックスバッファの生成と設定
	IndexCreate();
	// マテリアル用定数バッファの生成と初期化
	MaterialCreate();
	// 座標変換行列用定数バッファの生成と初期化
	TransformationCreate();

	// 指定したテクスチャを読み込み（TextureManagerで管理）
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	filePath = textureFilePath;

	// 読み込んだテクスチャサイズに合わせてスプライトサイズを調整
	AdjustTextureSize();
}

//更新
void Sprite::Update() {
	// アンカーポイントを考慮したスプライトの左・右・上・下座標を設定
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	// 左右反転処理
	if (isFlipX_) {
		left = -left;
		right = -right;
	}
	// 上下反転処理
	if (isFlipY_) {
		top = -top;
		bottom = -bottom;
	}

	// テクスチャのメタデータ（幅・高さ）を取得
	const DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetaData(filePath);

	// テクスチャのUV座標をピクセル単位から正規化座標に変換
	float tex_left = textureLeftTop.x / metadata.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
	float tex_top = textureLeftTop.y / metadata.height;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

	// 頂点データの位置・UV・法線情報を設定
	vertexData[0].position = { left,bottom,0.0f,1.0f };	// 左下
	vertexData[0].texcoord = { tex_left,tex_bottom };
	vertexData[0].normal = { 0.0f,0.0f,-1.0f };

	vertexData[1].position = { left,top,0.0f,1.0f };		// 左上
	vertexData[1].texcoord = { tex_left,tex_top };
	vertexData[1].normal = { 0.0f,0.0f,-1.0f };

	vertexData[2].position = { right,bottom,0.0f,1.0f };	// 右下
	vertexData[2].texcoord = { tex_right,tex_bottom };
	vertexData[2].normal = { 0.0f,0.0f,-1.0f };

	vertexData[3].position = { right,top,0.0f,1.0f };		// 右上
	vertexData[3].texcoord = { tex_right,tex_top };
	vertexData[3].normal = { 0.0f,0.0f,-1.0f };

	// Transform構造体を作成（スケール・回転・平行移動）
	Transform transform{ {size.x,size.y,1.0f},{0.0f,0.0f,rotation},{position.x,position.y,0.0f} };

	// ワールド行列を作成（スプライトの座標変換）
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

	// ビュー行列を単位行列（2D描画のためカメラの影響なし）
	Matrix4x4 viewMatrix = Math::MakeIdentity4x4();

	// 正射影行列を作成（スクリーン座標系で描画）
	Matrix4x4 projectionMatrix = Math::MakeOrthographicMatrix(
		0.0f, 0.0f,
		float(WindowsAPI::kClientWidth),
		float(WindowsAPI::kClientHeight),
		0.0f, 100.0f
	);

	// ワールド・ビュー・プロジェクション行列を計算し、GPUリソースに転送
	transformationMatrixData->WVP = Math::Multiply(worldMatrix, Math::Multiply(viewMatrix, projectionMatrix));
	// ワールド行列も単体で保持
	transformationMatrixData->World = worldMatrix;
}

//描画
void Sprite::Draw() {
	// 頂点バッファをコマンドリストに設定
	spriteBase->GetDxBase()->Getcommandlist()->IASetVertexBuffers(0, 1, &vertexBufferView);
	// インデックスバッファを設定
	spriteBase->GetDxBase()->Getcommandlist()->IASetIndexBuffer(&indexBufferView);
	// マテリアル定数バッファをセット（スロット0）
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(0, materialResource->GetGPUVirtualAddress());
	// 座標変換行列定数バッファをセット（スロット1）
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootConstantBufferView(1, transformationMatrixResource->GetGPUVirtualAddress());
	// テクスチャのSRVをセット（スロット2）
	spriteBase->GetDxBase()->Getcommandlist()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(filePath));
	// 描画命令を発行（インデックス6個、1インスタンス）
	spriteBase->GetDxBase()->Getcommandlist()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

//頂点データ作成
void Sprite::VertexDataCreate() {
	// 頂点バッファ用のリソースを作成（4頂点分）
	vertexResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(VertexData) * 4);

	// GPUへの頂点バッファの設定
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 4;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	// CPU側からデータを書き込めるようにマッピング
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
}

//index作成
void Sprite::IndexCreate() {
	// インデックスバッファ用のリソースを作成（6つ分）
	indexResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(uint32_t) * 6);

	// GPU側設定
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;

	// CPU側から書き込めるようにマッピング
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	// 四角形を2枚の三角形で構成するためのインデックスを設定
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;
}

//マテリアル作成
void Sprite::MaterialCreate() {
	// マテリアル用定数バッファを作成
	materialResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(Material));

	// CPU側からアクセスできるようにマッピング
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	// マテリアルの初期設定
	materialData->color = Vector4{ 1.0f,1.0f,1.0f,1.0f }; // 白（乗算用）
	materialData->enableLighting = false; // ライティング無効（2Dなので不要）
	materialData->uvTransform = Math::MakeIdentity4x4(); // UV変換行列を単位行列で初期化
}

//座標変換行列データ作成
void Sprite::TransformationCreate() {
	// 座標変換用定数バッファを作成
	transformationMatrixResource = spriteBase->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));

	// CPU側からアクセス可能にマッピング
	transformationMatrixResource->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	// 初期値として単位行列を設定
	transformationMatrixData->WVP = Math::MakeIdentity4x4();
	transformationMatrixData->World = Math::MakeIdentity4x4();
}

//テクスチャサイズをイメージに合わせる
void Sprite::AdjustTextureSize() {
	// テクスチャのメタデータを取得（幅・高さ）
	const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(filePath);

	// テクスチャサイズを設定
	textureSize.x = static_cast<float>(metadata.width);
	textureSize.y = static_cast<float>(metadata.height);

	// スプライト全体のサイズをテクスチャサイズに合わせる
	size = textureSize;
}
