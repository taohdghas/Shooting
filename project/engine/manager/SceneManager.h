#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include <memory>

//シーンを管理するクラス
class SceneManager
{
public:
	/// <summary>
	/// シングルトンインスタンスを取得する。
	/// - 初回呼び出し時にインスタンスを生成して返す。
	/// </summary>
	static SceneManager* GetInstance();
	/// <summary>
	/// 終了処理を行う。
	/// - 現在のシーンを Finalize() してからシングルトンインスタンスを破棄する。
	/// </summary>
	void Finalize();
	/// <summary>
	/// 毎フレームの更新処理を行う。
	/// - nextScene_ がセットされていれば現在のシーンを終了し（存在すれば）、nextScene_ を現在のシーンに切り替えて初期化する。
	/// - その後、現在のシーンの Update() を呼び出す。
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理を行う。
	/// - 現在のシーンの Draw() を呼び出す。
	/// </summary>
	void Draw();
public:
	/// <summary>
	/// 次のシーンを予約する。
	/// - 指定したシーン名に対応するシーンインスタンスをシーンファクトリで生成し、nextScene_ に格納する。
	/// - 呼び出し前に sceneFactory_ がセットされていること、および nextScene_ が未設定であることが前提。
	/// </summary>
	void ChangeScene(const std::string& sceneName);
	/// <summary>
	/// シーンファクトリーをセットする。
	/// - SceneManager はこのファクトリーを用いてシーンの生成を行う。
	/// </summary>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }
private:
	static SceneManager* instance;
	SceneManager* sceneManager_ = nullptr;
	//現在のシーン
	std::unique_ptr<BaseScene>scene_;
	//次のシーン
	std::unique_ptr<BaseScene>nextScene_;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
};

