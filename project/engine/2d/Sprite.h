#pragma once
#include "SrvManager.h"
#include "Struct.h"
#include "externals/DirectXTex/d3dx12.h"

//スプライト基盤クラス前方宣言
class SpriteBase;
//スプライト
class Sprite
{
public:
	//初期化
	/// <summary>
	/// スプライトを初期化する。
	/// テクスチャを読み込み、SRV を確保し、頂点・インデックス・マテリアル等の GPU リソースを作成する。
	/// </summary>
	/// <param name="spriteBase">描画共通設定を提供する <c>SpriteBase</c> のインスタンス。</param>
	/// <param name="textureFilePath">読み込むテクスチャのファイルパス。</param>
	void Initialize(SpriteBase*spriteBase,std::string textureFilePath);
    //更新
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - 座標変換行列を作成し、必要に応じて GPU 上のバッファへ書き込む。
	/// - テクスチャやサイズ変更があれば調整を行う。
	/// </summary>
	void Update();
	//描画
	/// <summary>
	/// スプライトを描画する。
	/// - 描画共通設定を行い、頂点・インデックス・マテリアルを用いて描画コマンドを発行する。
	/// </summary>
	void Draw();

	///getter///
	//位置
	/// <summary>スプライトの位置を取得する。</summary>
	/// <returns>スプライトの 2D 位置ベクトル。</returns>
	const Vector2& GetPosition()const { return position; }
	//回転
	/// <summary>スプライトの回転角（ラジアンまたは度は実装に依存）を取得する。</summary>
	/// <returns>回転角。</returns>
	float GetRotation()const { return rotation; }
	//色
	/// <summary>スプライトの描画色（RGBA）を取得する。</summary>
	/// <returns>現在設定されている色。</returns>
	const Vector4& GetColor()const { return materialData->color; }
	//サイズ
	/// <summary>スプライトの表示サイズを取得する。</summary>
	/// <returns>幅と高さを持つベクトル。</returns>
	const Vector2& GetSize()const { return size; }
	//アンカーポイント
	/// <summary>スプライトのアンカーポイントを取得する（左上原点からの比率など）。</summary>
	/// <returns>アンカーポイント。</returns>
	const Vector2& GetAnchorPoint()const { return anchorPoint; }
	//テクスチャ左上座標
	/// <summary>描画に使用するテクスチャの切り出し左上座標を取得する。</summary>
	/// <returns>テクスチャ左上座標。</returns>
	const Vector2& GettextureLeftTop()const { return textureLeftTop; }
	//テクスチャ切り出しサイズ
	/// <summary>描画に使用するテクスチャの切り出しサイズを取得する。</summary>
	/// <returns>テクスチャ切り出しサイズ。</returns>
	const Vector2& GettextureSize()const { return textureSize; }
	//左右フリップ
	/// <summary>水平方向に反転しているかを取得する。</summary>
	/// <returns>反転中であれば true。</returns>
	const bool& GetisFlipX()const { return isFlipX_; }
	//上下フリップ
	/// <summary>垂直方向に反転しているかを取得する。</summary>
	/// <returns>反転中であれば true。</returns>
	const bool& GetisFlipY()const { return isFlipY_; }

	///setter///
	//位置
	/// <summary>スプライトの位置を設定する。</summary>
	/// <param name="position">設定する位置ベクトル。</param>
	void SetPosition(const Vector2& position) { this->position = position; }
	//回転
	/// <summary>スプライトの回転角を設定する。</summary>
	/// <param name="rotation">設定する回転角。</param>
	void SetRotation(float rotation) { this->rotation = rotation; }
	//色
	/// <summary>スプライトの描画色（RGBA）を設定する。</summary>
	/// <param name="color">設定する色。</param>
	void SetColor(const Vector4& color) { materialData->color = color; }
	//サイズ
	/// <summary>スプライトの表示サイズを設定する。</summary>
	/// <param name="size">設定する幅と高さ。</param>
	void SetSize(const Vector2& size) { this->size = size; }
	//アンカーポイント
	/// <summary>スプライトのアンカーポイントを設定する。</summary>
	/// <param name="anchorPoint">設定するアンカーポイント。</param>
	void SetAnchorPoint(const Vector2& anchorPoint) { this->anchorPoint = anchorPoint; }
	//テクスチャ左上座標
	/// <summary>テクスチャの切り出し左上座標を設定する。</summary>
	/// <param name="textureLeftTop">設定する左上座標。</param>
	void SettextureLeftTop(const Vector2& textureLeftTop) { this->textureLeftTop = textureLeftTop; }
	//テクスチャ切り出しサイズ
	/// <summary>テクスチャの切り出しサイズを設定する。</summary>
	/// <param name="textureSize">設定する切り出しサイズ。</param>
	void SettextureSize(const Vector2& textureSize) { this->textureSize = textureSize; }
	//左右フリップ
	/// <summary>水平方向の反転設定を行う。</summary>
	/// <param name="isFlipX">反転する場合は true。</param>
	void SetisFlipX(bool isFlipX) { this->isFlipX_ = isFlipX; }
	//上下フリップ
	/// <summary>垂直方向の反転設定を行う。</summary>
	/// <param name="isFlipY">反転する場合は true。</param>
	void SetisFlipY(bool isFlipY) { this->isFlipY_ = isFlipY; }
private:
	SpriteBase* spriteBase = nullptr;
	SrvManager* srvManager = nullptr;
	//頂点データ作成
	/// <summary>
	/// 頂点バッファ用のデータを作成し、GPU 用バッファへコピーする。
	/// </summary>
	void VertexDataCreate();
	//index作成
	/// <summary>
	/// インデックスバッファを作成し、GPU に転送する。
	/// </summary>
	void IndexCreate();
	//マテリアル作成
	/// <summary>
	/// マテリアル（シェーダ定数など）用のバッファを作成する。
	/// </summary>
	void MaterialCreate();
	//座標変換行列データ作成
	/// <summary>
	/// ワールド・ビュー・プロジェクションを含む座標変換行列を構築し、GPU にアップロードする。
	/// </summary>
	void TransformationCreate();
	//テクスチャサイズをイメージに合わせる
	/// <summary>
	/// 読み込んだテクスチャの実サイズに合わせて内部のテクスチャ切り出しサイズ等を調整する。
	/// </summary>
	void AdjustTextureSize();
	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	uint32_t* indexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;

	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	//Sprite個々の座標
	Vector2 position = { 0.0f,0.0f };
	//サイズ
	Vector2 size = { 640.0f,360.0f };
	//アンカーポイント
	Vector2 anchorPoint = { 0.0f,0.0f };
	//テクスチャ左上座標
	Vector2 textureLeftTop = { 0.0f,0.0f };
	//テクスチャ切り出しサイズ
	Vector2 textureSize = { 100.0f,100.0f };
    //Sprite個々の回転角
	float rotation = 0.0f;
	//テクスチャ番号
	uint32_t textureIndex = 0;
	//左右フリップ
	bool isFlipX_ = false;
	//上下フリップ
	bool isFlipY_ = false;
	//ファイルパス
	std::string filePath;
};

