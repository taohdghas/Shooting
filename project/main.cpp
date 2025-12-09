#include "MyGame.h"
#include "Framework.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	std::unique_ptr<MyEngine::Framework> game = std::make_unique<MyEngine::MyGame>();

	game->Run();
	
	return 0;
}
