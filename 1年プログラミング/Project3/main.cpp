#include <DxLib.h>
#include "CLASS/SCEANE/SceneMng.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SceneMng sceneMng;
	sceneMng.Run();
	return 0;

}



