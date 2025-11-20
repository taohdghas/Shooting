#pragma once
#include "SrvManager.h"
#include "Struct.h"
#include "externals/DirectXTex/d3dx12.h"

// スプライト基盤クラス前方宣言
class SpriteBase;

// スプライトクラス
class Sprite {
public:
	/// <summary>
	/// スプライトを初期化する。
	/// テクスチャを読み込み、SRV を確保し、頂点・インデックス・マテリアル等の GPU リソースを作成する。
	/// </summary>
	/// <param name="sprite_base">描画共通設定を提供する <c>SpriteBase</c> のインスタンス。</param>
	/// <param name="texture_file_path">読み込むテクスチャのファイルパス。</param>
	void Initialize(SpriteBase* sprite_base, std::string texture_file_path);

	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 座標変換行列を作成し、必要に応じて GPU 上のバッファへ書き込む。
	/// - テクスチャやサイズ変更があれば調整を行う。
	/// </summary>
	void Update();

	/// <summary>
	/// スプライトを描画する。
	/// - 描画共通設定を行い、頂点・インデックス・マテリアルを用いて描画コマンドを発行する。
	/// </summary>
	void Draw();

	/// <summary>スプライトの位置を取得する。</summary>
	const Vector2& GetPosition() const { return position_; }
	/// <summary>スプライトの回転角（ラジアンまたは度は実装に依存）を取得する。</summary>
	float GetRotation() const { return rotation_; }
	/// <summary>スプライトの描画色（RGBA）を取得する。</summary>
	const Vector4& GetColor() const { return material_data_->color; }
	/// <summary>スプライトの表示サイズを取得する。</summary>
	const Vector2& GetSize() const { return size_; }
	/// <summary>スプライトのアンカーポイントを取得する（左上原点からの比率など）。</summary>
	const Vector2& GetAnchorPoint() const { return anchor_point_; }
	/// <summary>描画に使用するテクスチャの切り出し左上座標を取得する。</summary>
	const Vector2& GetTextureLeftTop() const { return texture_left_top_; }
	/// <summary>描画に使用するテクスチャの切り出しサイズを取得する。</summary>
	const Vector2& GetTextureSize() const { return texture_size_; }
	/// <summary>水平方向に反転しているかを取得する。</summary>
	const bool& GetIsFlipX() const { return is_flip_x_; }
	/// <summary>垂直方向に反転しているかを取得する。</summary>
	const bool& GetIsFlipY() const { return is_flip_y_; }

	/// <summary>スプライトの位置を設定する。</summary>
	void SetPosition(const Vector2& position) { position_ = position; }
	/// <summary>スプライトの回転角を設定する。</summary>
	void SetRotation(float rotation) { rotation_ = rotation; }
	/// <summary>スプライトの描画色（RGBA）を設定する。</summary>
	void SetColor(const Vector4& color) { material_data_->color = color; }
	/// <summary>スプライトの表示サイズを設定する。</summary>
	void SetSize(const Vector2& size) { size_ = size; }
	/// <summary>スプライトのアンカーポイントを設定する。</summary>
	void SetAnchorPoint(const Vector2& anchor_point) { anchor_point_ = anchor_point; }
	/// <summary>テクスチャの切り出し左上座標を設定する。</summary>
	void SetTextureLeftTop(const Vector2& texture_left_top) { texture_left_top_ = texture_left_top; }
	/// <summary>テクスチャの切り出しサイズを設定する。</summary>
	void SetTextureSize(const Vector2& texture_size) { texture_size_ = texture_size; }
	/// <summary>水平方向の反転設定を行う。</summary>
	void SetIsFlipX(bool is_flip_x) { is_flip_x_ = is_flip_x; }
	/// <summary>垂直方向の反転設定を行う。</summary>
	void SetIsFlipY(bool is_flip_y) { is_flip_y_ = is_flip_y; }

private:
	SpriteBase* sprite_base_ = nullptr;
	SrvManager* srv_manager_ = nullptr;

	// 頂点データ作成
	void VertexDataCreate();
	// インデックス作成
	void IndexCreate();
	// マテリアル作成
	void MaterialCreate();
	// 座標変換行列データ作成
	void TransformationCreate();
	// テクスチャサイズをイメージに合わせる
	void AdjustTextureSize();

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
	// テクスチャ番号
	uint32_t texture_index_ = 0;
	// 左右フリップ
	bool is_flip_x_ = false;
	// 上下フリップ
	bool is_flip_y_ = false;
	// ファイルパス
	std::string file_path_;
};

