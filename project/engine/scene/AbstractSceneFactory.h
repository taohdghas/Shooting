#pragma once
#include "BaseScene.h"
#include <string>
#include <memory>

namespace MyEngine {

    // シーンを作る抽象ファクトリクラス
    /// <summary>
    /// シーン名に応じたBaseScene派生クラスのインスタンス生成処理の抽象化
    /// シーン生成処理の共通インターフェースの提供
    /// SceneManager等からのシーン生成要求に応じて適切なシーンを返す仕組みの基盤
    /// 派生クラスで具体的なシーン生成ロジックを実装するための基底
    /// </summary>
    class AbstractSceneFactory {
    public:
        // 仮想デストラクタ
        virtual ~AbstractSceneFactory() = default;

        // シーン生成
        virtual std::unique_ptr<BaseScene> CreateScene(const std::string& scene_name) = 0;
    };
}