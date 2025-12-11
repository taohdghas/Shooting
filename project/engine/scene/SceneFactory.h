#pragma once
#include "AbstractSceneFactory.h"

namespace MyEngine {

	//シーンファクトリークラス
	class SceneFactory :public AbstractSceneFactory
	{
	public:
		/// <summary>
		/// シーン名に応じたシーンインスタンスを生成して返す。
		/// </summary>
		std::unique_ptr<BaseScene>CreateScene(const std::string& scene_name)override;
	};
}