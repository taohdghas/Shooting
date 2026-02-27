#pragma once
#include "SrvManager.h"
#include "Struct.h"
#include "externals/DirectXTex/d3dx12.h"

namespace MyEngine {

	// スプライト基盤クラス前方宣言
	class SpriteBase;

	// スプライトクラス
	/// <summary>
	/// 2Dスプライトの初期化、描画、更新処理の管理
	/// </summary>
	class Sprite {
	public:
		/// <summary>
		/// スプライトを初期化する。
		/// <param name="sprite_base">スプライト共通部のポインタ</param>
		/// <param name="texture_file_path">テクスチャファイルのパス</param>
		/// <returns>なし</returns>
		/// </summary>
		void Initialize(SpriteBase* sprite_base, const std::string& texture_file_path);

		/// <summary>
		/// 毎フレームの更新処理を行う
		/// <returns>なし</returns>
		/// </summary>
		void Update();

		/// <summary>
		/// スプライトを描画する。
		/// <returns>なし</returns>
		/// </summary>
		void Draw();

		/// <summary>スプライトの位置を取得する。
		/// <returns>スプライトの座標（Vector2）</returns>
		/// </summary>
		const Vector2& GetPosition() const { return position_; }
		/// <summary>スプライトの回転角を取得
		/// <returns>回転角（float）</returns>
		/// </summary>
		float GetRotation() const { return rotation_; }
		/// <summary>スプライトの描画色を取得
		/// <returns>色（Vector4）</returns>
		/// </summary>
		const Vector4& GetColor() const { return material_data_->color; }
		/// <summary>スプライトの表示サイズを取得
		/// <returns>サイズ（Vector2）</returns>
		/// </summary>
		const Vector2& GetSize() const { return size_; }
		/// <summary>スプライトのアンカーポイントを取得
		/// <returns>アンカーポイント（Vector2）</returns>
		/// </summary>
		const Vector2& GetAnchorPoint() const { return anchor_point_; }
		/// <summary>描画に使用するテクスチャの切り出し左上座標を取得
		/// <returns>左上座標（Vector2）</returns>
		/// </summary>
		const Vector2& GetTextureLeftTop() const { return texture_left_top_; }
		/// <summary>描画に使用するテクスチャの切り出しサイズを取得
		/// <returns>切り出しサイズ（Vector2）</returns>
		/// </summary>
		const Vector2& GetTextureSize() const { return texture_size_; }
		/// <summary>水平方向に反転しているかを取得
		/// <returns>反転状態（bool）</returns>
		/// </summary>
		const bool& GetIsFlipX() const { return is_flip_x_; }
		/// <summary>垂直方向に反転しているかを取得
		/// <returns>反転状態（bool）</returns>
		/// </summary>
		const bool& GetIsFlipY() const { return is_flip_y_; }

		/// <summary>スプライトの位置を設定。
		/// <param name="position">スプライトの座標（Vector2）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetPosition(const Vector2& position) { position_ = position; }
		/// <summary>スプライトの回転角を設定
		/// <param name="rotation">回転角（float）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetRotation(float rotation) { rotation_ = rotation; }
		/// <summary>スプライトの描画色（RGBA）を設定
		/// <param name="color">色（Vector4）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetColor(const Vector4& color) { material_data_->color = color; }
		/// <summary>スプライトの表示サイズを設定
		/// <param name="size">サイズ（Vector2）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetSize(const Vector2& size) { size_ = size; }
		/// <summary>スプライトのアンカーポイントを設定
		/// <param name="anchor_point">アンカーポイント（Vector2）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetAnchorPoint(const Vector2& anchor_point) { anchor_point_ = anchor_point; }
		/// <summary>テクスチャの切り出し左上座標を設定
		/// <param name="texture_left_top">左上座標（Vector2）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetTextureLeftTop(const Vector2& texture_left_top) { texture_left_top_ = texture_left_top; }
		/// <summary>テクスチャの切り出しサイズを設定
		/// <param name="texture_size">切り出しサイズ（Vector2）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetTextureSize(const Vector2& texture_size) { texture_size_ = texture_size; }
		/// <summary>水平方向の反転設定
		/// <param name="is_flip_x">反転状態（bool）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetIsFlipX(bool is_flip_x) { is_flip_x_ = is_flip_x; }
		/// <summary>垂直方向の反転設定
		/// <param name="is_flip_y">反転状態（bool）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetIsFlipY(bool is_flip_y) { is_flip_y_ = is_flip_y; }

	private:
		/// <summary>
        /// 頂点データ作成
        /// <returns>なし</returns>
        /// </summary>
		void VertexDataCreate();
		/// <summary>
		/// インデックス作成
		/// <returns>なし</returns>
		/// </summary>
		void IndexCreate();
		/// <summary>
		/// マテリアル作成
		/// <returns>なし</returns>
		/// </summary>
		void MaterialCreate();
		/// <summary>
		/// 座標変換行列データ作成
		/// <returns>なし</returns>
		/// </summary>
		void TransformationCreate();
		/// <summary>
		/// テクスチャサイズをイメージに合わせる
		/// <returns>なし</returns>
		/// </summary>
		void AdjustTextureSize();
	private:
		SpriteBase* sprite_base_ = nullptr;
		SrvManager* srv_manager_ = nullptr;

		// バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> index_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> material_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> transformation_matrix_resource_;

		// バッファリソース内のデータを指すポインタ
		VertexData* vertex_data_ = nullptr;
		uint32_t* index_data_ = nullptr;
		Material* material_data_ = nullptr;
		TransformationMatrix* transformation_matrix_data_ = nullptr;

		// バッファリソースの使い道を補足するバッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;
		D3D12_INDEX_BUFFER_VIEW index_buffer_view_;

		D3D12_GPU_DESCRIPTOR_HANDLE texture_srv_handle_gpu_;

		// スプライト個々の座標
		Vector2 position_ = { 0.0f, 0.0f };
		// サイズ
		Vector2 size_ = { 640.0f, 360.0f };
		// アンカーポイント
		Vector2 anchor_point_ = { 0.0f, 0.0f };
		// テクスチャ左上座標
		Vector2 texture_left_top_ = { 0.0f, 0.0f };
		// テクスチャ切り出しサイズ
		Vector2 texture_size_ = { 100.0f, 100.0f };
		// スプライト個々の回転角
		float rotation_ = 0.0f;
		//ニアクリップ座標
		float near_clip_ = 0.0f;
		//ファークリップ座標
		float far_clip_ = 100.0f;
		// テクスチャ番号
		uint32_t texture_index_ = 0;
		// 左右フリップ
		bool is_flip_x_ = false;
		// 上下フリップ
		bool is_flip_y_ = false;
		// ファイルパス
		std::string file_path_;
	};
}