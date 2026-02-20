#pragma once

namespace MyEngine {

	//シーンマネージャー前方宣言
	class SceneManager;
	//シーンの基底クラス
	/// <summary>
	/// ゲーム内の各シーンの共通インターフェースを提供
	/// シーンの初期化・終了・更新・描画・デバッグ処理の純粋仮想関数として定義
	/// シーンマネージャとの連携のためのインターフェース提供
	/// 派生クラスで具体的なシーン処理を実装するための基盤
	/// </summary>
	class BaseScene
	{
	public:
		/// <summary>
		//初期化
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		//終了
		virtual void Finalize() = 0;
		/// </summary>

		/// <summary>
		//更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		//描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		//デバック
		/// </summary>
		virtual void Debug() = 0;

		//仮想デストラクタ
		virtual ~BaseScene() = default;
	public:
		//シーンマネージャをシーンに貸す
		virtual void SetSceneManager(SceneManager* scene_manager) { sceneManager_ = scene_manager; }
	private:
		//シーンマネージャ
		SceneManager* sceneManager_ = nullptr;
	};
}