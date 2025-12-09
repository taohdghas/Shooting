#pragma once

namespace MyEngine {

	//シーンマネージャー前方宣言
	class SceneManager;
	//シーンの基底クラス
	class BaseScene
	{
	public:
		//初期化
		virtual void Initialize() = 0;
		//終了
		virtual void Finalize() = 0;
		//更新
		virtual void Update() = 0;
		//描画
		virtual void Draw() = 0;
		//デバック
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