#pragma once
#include "Struct.h"
#include <string>
#include <vector>
#include "externals/DirectXTex/d3dx12.h"
#include "camera.h"

namespace MyEngine {

	// モデルクラス前方宣言
	class Model;
	// オブジェクト共通部前方宣言
	class Object3dBase;

	// 3Dオブジェクトクラス
	/// <summary>
	/// 3Dオブジェクトの初期化、更新、描画処理の管理
	/// モデルデータやトランスフォームの管理
	/// マテリアルやライティングの設定・制御
	/// カメラとの連携およびカメラデータの管理
	/// DirectX 12リソースの生成と管理
	/// デバッグ用の更新処理の提供
	/// </summary>
	class Object3d {
	public:
		/// <summary>
        /// 初期化
        /// <param name="object3d_base">3Dオブジェクト共通部のポインタ</param>
        /// <returns>なし</returns>
        /// </summary>
		void Initialize(Object3dBase* object3d_base);

		/// <summary>
		/// 毎フレームの更新処理
		/// <returns>なし</returns>
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理
		/// <returns>なし</returns>
		/// </summary>
		void Draw();

		/// <summary>
		/// デバッグ用の更新処理
		/// <returns>なし</returns>
		/// </summary>
		void DebugUpdate();

		/// <summary>
		/// 表示するモデルを設定
		/// <param name="file_path">モデルファイルのパス</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetModel(const std::string& file_path);

		/// <summary>
		/// スケールを設定
		/// <param name="scale">スケール値（Vector3）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetScale(const Vector3& scale) { transform_.scale = scale; }

		/// <summary>
		/// 回転を設定。
		/// <param name="rotate">回転値（Vector3）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

		/// <summary>
		/// 座標を設定。
		/// <param name="translate">座標値（Vector3）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetTranslate(const Vector3& translate) { transform_.translate = translate; }

		/// <summary>
		/// 使用するカメラを設定
		/// <param name="camera">カメラのポインタ</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetCamera(Camera* camera) { camera_ = camera; }

		/// <summary>
		/// マテリアルの色を設定
		/// <param name="color">色（Vector4）</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetColor(const Vector4& color);

		/// <summary>
		/// ライティングの有効/無効を設定
		/// <param name="enable">有効ならtrue、無効ならfalse</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetLight(bool enable) { enable_lighting_ = enable; }

		/// <summary>
		/// 平行光の有効/無効を設定
		/// <param name="enable">有効なら1、無効なら0</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetDirectionalLightEnable(int enable) { directional_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// 点光源の有効/無効を設定
		/// <param name="enable">有効なら1、無効なら0</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetPointLightEnable(int enable) { point_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// スポットライトの有効/無効を設定
		/// <param name="enable">有効なら1、無効なら0</param>
		/// <returns>なし</returns>
		/// </summary>
		void SetSpotLightEnable(int enable) { spot_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// スケールを取得
		/// <returns>スケール値（Vector3）</returns>
		/// </summary>
		const Vector3& GetScale() const { return transform_.scale; }

		/// <summary>
		/// 回転を取得
		/// <returns>回転値（Vector3）</returns>
		/// </summary>
		const Vector3& GetRotate() const { return transform_.rotate; }

		/// <summary>
		/// 座標を取得
		/// <returns>座標値（Vector3）</returns>
		/// </summary>
		const Vector3& GetTranslate() const { return transform_.translate; }

	private:

		/// <summary>
		/// マテリアル情報を作成・初期化
		/// <returns>なし</returns>
		/// </summary>
		void MaterialCreate();

		/// <summary>
		/// 座標変換行列データを作成
		/// <returns>なし</returns>
		/// </summary>
		void TransformationCreate();

		/// <summary>
		/// 平行光源のデータを作成・更新
		/// <returns>なし</returns>
		/// </summary>
		void DirectionalLightCreate();

		/// <summary>
		/// カメラ関連のデータを作成・更新
		/// <returns>なし</returns>
		/// </summary>
		void CameraDataCreate();

		/// <summary>
		/// 点光源のデータを作成・更新
		/// <returns>なし</returns>
		/// </summary>
		void PointLightCreate();

		/// <summary>
		/// スポットライトのデータを作成・更新
		/// <returns>なし</returns>
		/// </summary>
		void SpotLightCreate();

	private:
		Model* model_ = nullptr;
		Object3dBase* object3d_base_ = nullptr;
		Camera* camera_ = nullptr;
		ModelData model_data_;

		Transform transform_ = { 0.0f,0.0f,0.0f };
		Transform camera_transform_ = { 0.0f,0.0f,0.0f };

		// バッファリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertex_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> material_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> transformation_matrix_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> directional_light_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> camera_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> point_light_resource_;
		Microsoft::WRL::ComPtr<ID3D12Resource> spot_light_resource_;

		// バッファリソース内のデータを指すポインタ
		VertexData* vertex_data_ = nullptr;
		Material* material_data_ = nullptr;
		TransformationMatrix* transformation_matrix_data_ = nullptr;
		DirectionalLight* directional_light_ = nullptr;
		CameraForGPU* camera_data_ = nullptr;
		PointLight* point_light_ = nullptr;
		SpotLight* spot_light_ = nullptr;

		// バッファリソースの使い道を補足するバッファビュー
		D3D12_VERTEX_BUFFER_VIEW vertex_buffer_view_;

		// ライト有効フラグ
		bool enable_lighting_ = false;
	};
}