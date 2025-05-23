#include "JsonManager.h"
#include <string>
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
void JsonManager::LoadJsonFile() {
	//連続してフルパスを得る
	const std::string fullpath =;

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
	

}