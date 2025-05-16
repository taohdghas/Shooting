#pragma once
class JsonManager
{
public:
	//シングルトンインスタンスの取得
	static JsonManager* GetInstance();
	//Jsonファイル読み込み
	void LoadJsonFile();


private:
	static JsonManager* instance;
};

