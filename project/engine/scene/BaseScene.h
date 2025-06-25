#pragma once

class SceneManager;
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
	//仮想デストラクタ
	virtual ~BaseScene() = default;
public:
	//シーンマネージャをシーンに貸す
	virtual void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
private:
	//シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
};

