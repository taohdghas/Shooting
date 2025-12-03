#pragma once
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"
#include "camera.h"

//モデルクラス前方宣言
class Model;
//オブジェクト共通部前方宣言
class Object3dBase;
//3Dオブジェクト
class Object3d
{
public:
	/// <summary>
	/// 初期化を行う。
	/// Object3dBase の参照を受け取り、必要なリソースやデフォルト状態を設定する。
	/// </summary>
	/// <param name="object3dBase">描画共通設定を提供する <c>Object3dBase</c> のポインタ。</param>
	void Initialize(Object3dBase*object3dBase);
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - トランスフォームやライト・カメラデータの更新を行い、GPU へ反映する準備をする。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - 内部のメッシュ・マテリアル情報を用いて描画コマンドを発行する。
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ用の更新処理を行う。
	/// - デバッグ表示やパラメータ調整処理をここで扱う。
	/// </summary>
	void DebugUpdate();

	///setter///

	/// <summary>
	/// 表示するモデルを設定する。
	/// 指定されたファイルパスを基にモデルリソースを読み込み、内部の Model および ModelData を更新する。
	/// </summary>
	/// <param name="filePath">モデルファイルのパス。</param>
	void SetModel(const std::string& filePath);
	/// <summary>
	/// スケールを設定する。
	/// </summary>
	/// <param name="scale">設定するスケール値。</param>
	void SetScale(const Vector3& scale) { this->transform_.scale = scale; }
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotate">設定する回転値。</param>
	void SetRotate(const Vector3& rotate) { this->transform_.rotate = rotate; }
	/// <summary>
	/// 平行移動（座標）を設定する。
	/// </summary>
	/// <param name="translate">設定する座標値。</param>
	void SetTranslate(const Vector3& translate) { this->transform_.translate = translate; }
	/// <summary>
	/// 使用するカメラを設定する。
	/// </summary>
	/// <param name="camera">描画に用いる <c>Camera</c> のポインタ。</param>
	void SetCamera(Camera* camera) { this->camera = camera; }
	/// <summary>
	/// マテリアルの色を設定する。
	/// GPU 上のマテリアルデータに色を反映することを想定する。
	/// </summary>
	/// <param name="color">設定する RGBA 色。</param>
	void SetColor(const Vector4& color);
	/// <summary>
	/// ライティングの有効/無効を設定する。
	/// </summary>
	/// <param name="enable">true でライティング有効、false で無効。</param>
	void SetLight(bool enable) { enableLighting = enable; }
	/// <summary>
	/// 平行光（DirectionalLight）の有効/無効を設定する。
	/// </summary>
	/// <param name="enable">0 で無効、非ゼロで有効とする整数値。</param>
	void SetDirectionalLightEnable(int enable) { directionalLight->enable = enable ? 1 : 0; }
	/// <summary>
	/// 点光源（PointLight）の有効/無効を設定する。
	/// </summary>
	/// <param name="enable">0 で無効、非ゼロで有効とする整数値。</param>
	void SetPointLightEnable(int enable) { pointLight->enable = enable ? 1 : 0; }
	/// <summary>
	/// スポットライト（SpotLight）の有効/無効を設定する。
	/// </summary>
	/// <param name="enable">0 で無効、非ゼロで有効とする整数値。</param>
	void SetSpotLightEnable(int enable) { spotLight->enable = enable ? 1 : 0; }
	///getter///

	/// <summary>
	/// スケールを取得する。
	/// </summary>
	/// <returns>現在のスケール。</returns>
	const Vector3& GetScale()const { return transform_.scale; }
	/// <summary>
	/// 回転を取得する。
	/// </summary>
	/// <returns>現在の回転。</returns>
	const Vector3& GetRotate()const { return transform_.rotate; }
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>現在の座標（平行移動）。</returns>
	const Vector3& GetTranslate()const { return transform_.translate; }
private:
	/// <summary>
	/// 座標変換行列データを作成する。
	/// - スケール・回転・平行移動を組み合わせてトランスフォーム行列を構築し、GPU 用バッファへ書き込む処理を行う。
	/// </summary>
	void TransformationCreate();
	/// <summary>
	/// 平行光源のデータを作成・更新する。
	/// - DirectionalLight 構造体を初期化し、GPU バッファに反映する処理を行う。
	/// </summary>
	void DirectionalLightCreate();
	/// <summary>
	/// カメラ関連のデータを作成・更新する。
	/// - カメラのワールド・ビュー・プロジェクション行列等を GPU 用データに格納する。
	/// </summary>
	void CameraDataCreate();
	/// <summary>
	/// 点光源（PointLight）のデータを作成・更新する。
	/// </summary>
	void PointLightCreate();
	/// <summary>
	/// スポットライト（SpotLight）のデータを作成・更新する。
	/// </summary>
	void SpotLightCreate();
private:
	Model* model_ = nullptr;
	Object3dBase* object3dBase_ = nullptr;
	Camera* camera = nullptr;
	//objファイルのデータ
	ModelData modelData_;
	
	Transform transform_;
	Transform cameraTransform_;

	//バッファリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResource;
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>DirectionalLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>cameraResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>PointLightResource;
	Microsoft::WRL::ComPtr<ID3D12Resource>SpotLightResource;
	//バッファリソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	DirectionalLight* directionalLight = nullptr;
	CameraForGPU* cameraData = nullptr;
	PointLight* pointLight = nullptr;
	SpotLight* spotLight = nullptr;
	//バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	//ライト有効フラグ
	bool enableLighting = false;
};

