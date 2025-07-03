#include "Skybox.h"

//初期化
void Skybox::Initialize() {


}

//描画
void Skybox::Draw() {

}

//頂点データ作成
void Skybox::CreateVertex() {
	//右面
	vertexData[0].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[1].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[2].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[3].position = { 1.0f,-1.0f,-1.0f,1.0f };
	//左面
	vertexData[4].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[5].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[6].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[7].position = { -1.0f,-1.0f,1.0f,1.0f };
	//前面
	vertexData[8].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[9].position = { 1.0f,1.0f,1.0f,1.0f };
	vertexData[10].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[11].position = { 1.0f,-1.0f,1.0f,1.0f };
    //後面
	vertexData[12].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[13].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[14].position = { 1.0f,-1.0f,-1.0f,1.0f };
	vertexData[15].position = { -1.0f,-1.0f,-1.0f,1.0f };
	//上面
	vertexData[16].position = { -1.0f,1.0f,-1.0f,1.0f };
	vertexData[17].position = { 1.0f,1.0f,-1.0f,1.0f };
	vertexData[18].position = { -1.0f,1.0f,1.0f,1.0f };
	vertexData[19].position = { 1.0f,1.0f,1.0f,1.0f };
	//下面
	vertexData[20].position = { -1.0f,-1.0f,1.0f,1.0f };
	vertexData[21].position = { 1.0f,-1.0f,1.0f,1.0f };
	vertexData[22].position = { -1.0f,-1.0f,-1.0f,1.0f };
	vertexData[23].position = { 1.0f,-1.0f,-1.0f,1.0f };
}