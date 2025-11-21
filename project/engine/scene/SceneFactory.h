#pragma once
#include "AbstractSceneFactory.h"

//シーンファクトリークラス
class SceneFactory:public AbstractSceneFactory
{
public:
	/// <summary>
	/// シーン名に応じたシーンインスタンスを生成して返す。
	/// </summary>
	/// <param name="sceneName">生成するシーンを識別する文字列（大文字で比較される実装）。</param>
	/// <returns>生成された <c>BaseScene</c> のユニークポインタ、該当が無ければ <c>nullptr</c>。</returns>
	std::unique_ptr<BaseScene>CreateScene(const std::string& scene_name)override;
};
