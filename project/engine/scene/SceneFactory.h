#pragma once
#include "AbstractSceneFactory.h"

namespace MyEngine {

	//シーンファクトリークラス
    /// <summary>
    /// シーン名に応じて適切なBaseScene派生クラスのインスタンスを生成・返却
    /// AbstractSceneFactoryを継承し、具体的なシーン生成ロジックを実装
    /// SceneManager等からのシーン生成要求に対応
    /// ゲーム内のシーン遷移や管理のためのシーン生成基盤の提供
    /// </summary>
	class SceneFactory :public AbstractSceneFactory
	{
	public:
		/// <summary>
		/// シーン名に応じたシーンインスタンスを生成して返す。
		/// </summary>
		std::unique_ptr<BaseScene>CreateScene(const std::string& scene_name)override;
	};
}