#include <DxLib.h>
#include "_debug/_debug/_DebugDispOut.h"
#include "_debug/_debug/_DebugConOut.h"
#include "class/scene/SceneMng.h"
#include "class/Vector2D.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//stl = standard template library
	//égÇ¢ÇΩÇ¢å^Ç<>ÇÃíÜÇ…èëÇ≠
	//Vector2DTmp<float> vec;
	/*Vector2DFlot vec1;
	Vector2DFlot vec2;
	
	auto VecAns = vec1 + vec2;
	TRACE("%f,%f", VecAns.x, VecAns.y);*/

	lpSceneMng.Run();

	return 0;
}