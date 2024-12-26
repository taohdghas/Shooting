#pragma once

enum SCENE{TITLE,STAGE,CLEAR};
class IScene
{
protected:
	//シーン番号を管理する
	static int sceneNo;
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual ~IScene();

	int GetSceneNo() { return sceneNo; }
};

