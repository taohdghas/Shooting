#include "Sprite.h"
#include "SpriteBase.h"
#include "TextureManager.h"
#include "MyMath.h"
#include "Transform.h"

namespace MyEngine {

	//初期化
	void Sprite::Initialize(SpriteBase* sprite_base, const std::string& texture_file_path) {
		// スプライトの初期化処理
		sprite_base_ = sprite_base;

		// 頂点バッファの生成と設定
		VertexDataCreate();
		// インデックスバッファの生成と設定
		IndexCreate();
		// マテリアル用定数バッファの生成と初期化
		MaterialCreate();
		// 座標変換行列用定数バッファの生成と初期化
		TransformationCreate();

		// 指定したテクスチャを読み込み（TextureManagerで管理）
		TextureManager::GetInstance()->LoadTexture(texture_file_path);
		file_path_ = texture_file_path;

		// 読み込んだテクスチャサイズに合わせてスプライトサイズを調整
		AdjustTextureSize();
	}

	//更新
	void Sprite::Update() {
		// アンカーポイントを考慮したスプライトの左・右・上・下座標を設定
		float left = kSpriteLocalMin - anchor_point_.x;
		float right = kSpriteLocalMax - anchor_point_.x;
		float top = kSpriteLocalMin - anchor_point_.y;
		float bottom = kSpriteLocalMax - anchor_point_.y;

		// 左右反転処理
		if (is_flip_x_) {
			left = -left;
			right = -right;
		}
		// 上下反転処理
		if (is_flip_y_) {
			top = -top;
			bottom = -bottom;
		}

		// テクスチャのメタデータ（幅・高さ）を取得
		const DirectX::TexMetadata& metadata =
			TextureManager::GetInstance()->GetMetaData(file_path_);

		// テクスチャのUV座標をピクセル単位から正規化座標に変換
		float tex_left = texture_left_top_.x / metadata.width;
		float tex_right = (texture_left_top_.x + texture_size_.x) / metadata.width;
		float tex_top = texture_left_top_.y / metadata.height;
		float tex_bottom = (texture_left_top_.y + texture_size_.y) / metadata.height;

		// 頂点データの位置・UV・法線情報を設定
		vertex_data_[0].position = { left, bottom, kSpriteLocalZ, kSpriteLocalW };   // 左下
		vertex_data_[0].texcoord = { tex_left, tex_bottom };
		vertex_data_[0].normal = { kSpriteLocalZ, kSpriteLocalZ,kSpriteNormalZ };

		vertex_data_[1].position = { left, top, kSpriteLocalZ, kSpriteLocalW };      // 左上
		vertex_data_[1].texcoord = { tex_left, tex_top };
		vertex_data_[1].normal = { kSpriteLocalZ, kSpriteLocalZ,kSpriteNormalZ };

		vertex_data_[2].position = { right, bottom,kSpriteLocalZ, kSpriteLocalW };  // 右下
		vertex_data_[2].texcoord = { tex_right, tex_bottom };
		vertex_data_[2].normal = { kSpriteLocalZ, kSpriteLocalZ,kSpriteNormalZ };

		vertex_data_[3].position = { right, top, kSpriteLocalZ, kSpriteLocalW };     // 右上
		vertex_data_[3].texcoord = { tex_right, tex_top };
		vertex_data_[3].normal = { kSpriteLocalZ, kSpriteLocalZ,kSpriteNormalZ };

		// Transform構造体を作成（スケール・回転・平行移動）
		Transform transform{ {size_.x, size_.y, 1.0f}, {0.0f, 0.0f, rotation_}, {position_.x, position_.y, 0.0f} };

		// ワールド行列を作成（スプライトの座標変換）
		Matrix4x4 world_matrix = Math::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);

		// ビュー行列を単位行列（2D描画のためカメラの影響なし）
		Matrix4x4 view_matrix = Math::MakeIdentity4x4();

		// 正射影行列を作成（スクリーン座標系で描画）
		Matrix4x4 projection_matrix = Math::MakeOrthographicMatrix(
			0.0f, 0.0f,
			float(WindowsApi::kClientWidth),
			float(WindowsApi::kClientHeight),
			near_clip_, far_clip_
		);

		// ワールド・ビュー・プロジェクション行列を計算し、GPUリソースに転送
		transformation_matrix_data_->WVP = Math::Multiply(world_matrix, Math::Multiply(view_matrix, projection_matrix));
		// ワールド行列も単体で保持
		transformation_matrix_data_->World = world_matrix;
	}

	//描画
	void Sprite::Draw() {
		// 頂点バッファをコマンドリストに設定
		sprite_base_->GetDxBase()->GetCommandList()->IASetVertexBuffers(0, 1, &vertex_buffer_view_);
		// インデックスバッファを設定
		sprite_base_->GetDxBase()->GetCommandList()->IASetIndexBuffer(&index_buffer_view_);
		// マテリアル定数バッファをセット（スロット0）
		sprite_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(0, material_resource_->GetGPUVirtualAddress());
		// 座標変換行列定数バッファをセット（スロット1）
		sprite_base_->GetDxBase()->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformation_matrix_resource_->GetGPUVirtualAddress());
		// テクスチャのSRVをセット（スロット2）
		sprite_base_->GetDxBase()->GetCommandList()->SetGraphicsRootDescriptorTable(2, TextureManager::GetInstance()->GetSrvHandleGPU(file_path_));
		// 描画命令を発行（インデックス6個、1インスタンス）
		sprite_base_->GetDxBase()->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	//頂点データ作成
	void Sprite::VertexDataCreate() {
		// 頂点バッファ用のリソースを作成（4頂点分）
		vertex_resource_ = sprite_base_->GetDxBase()->CreateBufferResource(sizeof(VertexData) * 4);

		// GPUへの頂点バッファの設定
		vertex_buffer_view_.BufferLocation = vertex_resource_->GetGPUVirtualAddress();
		vertex_buffer_view_.SizeInBytes = sizeof(VertexData) * 4;
		vertex_buffer_view_.StrideInBytes = sizeof(VertexData);

		// CPU側からデータを書き込めるようにマッピング
		vertex_resource_->Map(0, nullptr, reinterpret_cast<void**>(&vertex_data_));
	}

	//index作成
	void Sprite::IndexCreate() {
		// インデックスバッファ用のリソースを作成（6つ分）
		index_resource_ = sprite_base_->GetDxBase()->CreateBufferResource(sizeof(uint32_t) * 6);

		// GPU側設定
		index_buffer_view_.BufferLocation = index_resource_->GetGPUVirtualAddress();
		index_buffer_view_.SizeInBytes = sizeof(uint32_t) * 6;
		index_buffer_view_.Format = DXGI_FORMAT_R32_UINT;

		// CPU側から書き込めるようにマッピング
		index_resource_->Map(0, nullptr, reinterpret_cast<void**>(&index_data_));

		// 四角形を2枚の三角形で構成するためのインデックスを設定
		index_data_[0] = 0; index_data_[1] = 1; index_data_[2] = 2;
		index_data_[3] = 1; index_data_[4] = 3; index_data_[5] = 2;
	}

	//マテリアル作成
	void Sprite::MaterialCreate() {
		// マテリアル用定数バッファを作成
		material_resource_ = sprite_base_->GetDxBase()->CreateBufferResource(sizeof(Material));

		// CPU側からアクセスできるようにマッピング
		material_resource_->Map(0, nullptr, reinterpret_cast<void**>(&material_data_));

		// マテリアルの初期設定
		material_data_->color = Vector4{ 1.0f, 1.0f, 1.0f, 1.0f }; // 白（乗算用）
		material_data_->enableLighting = false; // ライティング無効（2Dなので不要）
		material_data_->uvTransform = Math::MakeIdentity4x4(); // UV変換行列を単位行列で初期化
	}

	//座標変換行列データ作成
	void Sprite::TransformationCreate() {
		// 座標変換用定数バッファを作成
		transformation_matrix_resource_ = sprite_base_->GetDxBase()->CreateBufferResource(sizeof(TransformationMatrix));

		// CPU側からアクセス可能にマッピング
		transformation_matrix_resource_->Map(0, nullptr, reinterpret_cast<void**>(&transformation_matrix_data_));

		// 初期値として単位行列を設定
		transformation_matrix_data_->WVP = Math::MakeIdentity4x4();
		transformation_matrix_data_->World = Math::MakeIdentity4x4();
	}

	//テクスチャサイズをイメージに合わせる
	void Sprite::AdjustTextureSize() {
		// テクスチャのメタデータを取得（幅・高さ）
		const DirectX::TexMetadata& metadata = TextureManager::GetInstance()->GetMetaData(file_path_);

		// テクスチャサイズを設定
		texture_size_.x = static_cast<float>(metadata.width);
		texture_size_.y = static_cast<float>(metadata.height);

		// スプライト全体のサイズをテクスチャサイズに合わせる
		size_ = texture_size_;
	}
}