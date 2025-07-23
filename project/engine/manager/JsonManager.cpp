#include "JsonManager.h"
#include <fstream>
#include <cassert>
#include <json.hpp>

JsonManager* JsonManager::instance = nullptr;

//シングルトンインスタンスの取得
JsonManager* JsonManager::GetInstance() {
	if (instance == nullptr) {
		instance = new JsonManager;
	}
	return instance;
}

//Jsonファイルの読み込み
LevelData* JsonManager::LoadJsonFile(const std::string& filename) {
	//連続してフルパスを得る
	const std::string fullpath = "resources/levels/" + filename + ".json";

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//レベルデータ格納用インスタンスを生成
	LevelData* levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));


		if (object.contains("disabled_flag")) {
			//有効無効フラグ
			bool disabled = object["disabled_flag"].get<bool>();
			if (disabled) {
				//配置しない
				continue;
			}
		}

		//種別を取得
		std::string type = object["type"].get<std::string>();

		//種類ごとの処理
		//MESH
		if (type.compare("MESH") == 0) {
			//要素追加
			levelData->objects.emplace_back(LevelData::ObjectData{});
			//追加した要素の参照を得る
			LevelData::ObjectData& objectData = levelData->objects.back();

			if (object.contains("file_name")) {
				//ファイル名
				objectData.fileName = object["file_name"];
			}

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			objectData.translation.x = (float)transform["translation"][0];
			objectData.translation.y = (float)transform["translation"][2];
			objectData.translation.z = (float)transform["translation"][1];
			//回転角
			objectData.rotation.x = -(float)transform["rotation"][0];
			objectData.rotation.y = -(float)transform["rotation"][2];
			objectData.rotation.z = -(float)transform["rotation"][1];
			//スケーリング
			objectData.scaling.x = (float)transform["scaling"][0];
			objectData.scaling.y = (float)transform["scaling"][2];
			objectData.scaling.z = (float)transform["scaling"][1];
		}
		//自キャラ発生ポイント
		else if (type.compare("PlayerSpawn") == 0) {
			PlayerSpawnData playerSpawn{};

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			playerSpawn.translation.x = (float)transform["translation"][0];
			playerSpawn.translation.y = (float)transform["translation"][2];
			playerSpawn.translation.z = (float)transform["translation"][1];
			//回転角
			playerSpawn.rotation.x = -(float)transform["rotation"][0];
			playerSpawn.rotation.y = -(float)transform["rotation"][2];
			playerSpawn.rotation.z = -(float)transform["rotation"][1];
			//スケーリング
			playerSpawn.scaling.x = (float)transform["scaling"][0];
			playerSpawn.scaling.y = (float)transform["scaling"][2];
			playerSpawn.scaling.z = (float)transform["scaling"][1];

			levelData->players.push_back(playerSpawn);
		}
		//敵キャラ発生ポイント
		else if (type.compare("EnemySpawn") == 0) {
			EnemySpawnData enemySpawn{};

			//トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			//平行移動
			enemySpawn.translation.x = (float)transform["translation"][0];
			enemySpawn.translation.y = (float)transform["translation"][2];
			enemySpawn.translation.z = (float)transform["translation"][1];
			//回転角
			enemySpawn.rotation.x = -(float)transform["rotation"][0];
			enemySpawn.rotation.y = -(float)transform["rotation"][2];
			enemySpawn.rotation.z = -(float)transform["rotation"][1];
			//スケーリング
			enemySpawn.scaling.x = (float)transform["scaling"][0];
			enemySpawn.scaling.y = (float)transform["scaling"][2];
			enemySpawn.scaling.z = (float)transform["scaling"][1];

			levelData->enemies.push_back(enemySpawn);

		}
		//オブジェクトを再帰関数にまとめ再帰呼出で枝を走査する
		if (object.contains("children")) {

		}


	}
	return levelData;
}