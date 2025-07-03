#pragma once
#include "DirectXBase.h"
#include "Struct.h"

class Skybox
{
public:
	//初期化
	void Initialize();
	//描画
	void Draw();
	//頂点作成
	void CreateVertex();

private:
	VertexData* vertexData = nullptr;
};

