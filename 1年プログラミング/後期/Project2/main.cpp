#include <memory>
#include <Dxlib.h>
#include "Scene/SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow)

{
	////Baseクラスをユニークポインタで管理するという型　中身がない
	//std::unique_ptr<BaseScene> scene;
	////①宣言後に中身を取得
	//scene.reset(new TitleScene());
	//SceneMng::Create();
	SceneMng::GetInstance().Run();
	
	//delete scene;		//メモリリークを防ぐためにdelteする
	return 0;
}