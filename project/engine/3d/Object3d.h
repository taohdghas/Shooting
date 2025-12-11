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
	class Object3d {
	public:
		/// <summary>
		/// 初期化を行う。
		/// </summary>
		void Initialize(Object3dBase* object3d_base);

		/// <summary>
		/// 毎フレームの更新処理を行う。
		/// </summary>
		void Update();

		/// <summary>
		/// 描画処理を行う。
		/// </summary>
		void Draw();

		/// <summary>
		/// デバッグ用の更新処理を行う。
		/// </summary>
		void DebugUpdate();

		/// <summary>
		/// 表示するモデルを設定する。
		/// </summary>
		void SetModel(const std::string& file_path);

		/// <summary>
		/// スケールを設定する。
		/// </summary>
		void SetScale(const Vector3& scale) { transform_.scale = scale; }

		/// <summary>
		/// 回転を設定する。
		/// </summary>
		void SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }

		/// <summary>
		/// 平行移動（座標）を設定する。
		/// </summary>
		void SetTranslate(const Vector3& translate) { transform_.translate = translate; }

		/// <summary>
		/// 使用するカメラを設定する。
		/// </summary>
		void SetCamera(Camera* camera) { camera_ = camera; }

		/// <summary>
		/// マテリアルの色を設定する。
		/// </summary>
		void SetColor(const Vector4& color);

		/// <summary>
		/// ライティングの有効/無効を設定する。
		/// </summary>
		void SetLight(bool enable) { enable_lighting_ = enable; }

		/// <summary>
		/// 平行光（DirectionalLight）の有効/無効を設定する。
		/// </summary>
		void SetDirectionalLightEnable(int enable) { directional_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// 点光源（PointLight）の有効/無効を設定する。
		/// </summary>
		void SetPointLightEnable(int enable) { point_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// スポットライト（SpotLight）の有効/無効を設定する。
		/// </summary>
		void SetSpotLightEnable(int enable) { spot_light_->enable = enable ? 1 : 0; }

		/// <summary>
		/// スケールを取得する。
		/// </summary>
		const Vector3& GetScale() const { return transform_.scale; }

		/// <summary>
		/// 回転を取得する。
		/// </summary>
		const Vector3& GetRotate() const { return transform_.rotate; }

		/// <summary>
		/// 座標を取得する。
		/// </summary>
		const Vector3& GetTranslate() const { return transform_.translate; }

	private:

		/// <summary>
		/// マテリアル情報を作成・初期化する。
		/// </summary>
		void MaterialCreate();

		/// <summary>
		/// 座標変換行列データを作成する。
		/// </summary>
		void TransformationCreate();

		/// <summary>
		/// 平行光源のデータを作成・更新する。
		/// </summary>
		void DirectionalLightCreate();

		/// <summary>
		/// カメラ関連のデータを作成・更新する。
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
		Object3dBase* object3d_base_ = nullptr;
		Camera* camera_ = nullptr;
		ModelData model_data_;

		Transform transform_;
		Transform camera_transform_;

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
