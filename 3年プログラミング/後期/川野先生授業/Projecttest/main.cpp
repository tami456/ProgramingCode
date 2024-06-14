#include <DxLib.h>
#include"Scene/SceneManager.h"
#include"Scene/TitleScene.h"
#include"Input.h"

using namespace std;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager sceneManager;
	sceneManager.ChangeScene(make_shared<TitleScene>(sceneManager));

	Input input;

	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();
		input.Update();
		sceneManager.Update(input);
		sceneManager.Draw();

		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}