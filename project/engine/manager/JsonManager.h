#pragma once
#include "Struct.h"
#include <string>
#include <vector>

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
