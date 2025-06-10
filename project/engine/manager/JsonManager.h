#pragma once
#include "Struct.h"
#include <string>
#include <vector>

//レベルデータ
struct LevelData {
	//オブジェクト1個分のデータ
	struct ObjectData {
		//ファイル名
		std::string fileName;
		Vector3 translation;
		Vector3 rotation;
		Vector3 scaling;
	};
	//オブジェクトのコンテナ
	std::vector<ObjectData>objects;
};

class JsonManager
{

public:
	//シングルトンインスタンスの取得
	static JsonManager* GetInstance();
	//Jsonファイル読み込み
	LevelData* LoadJsonFile(const std::string&filename);

private:
	static JsonManager* instance;

	static const std::string kDefaultBaseDirectory;
	static const std::string kExtension;
};
