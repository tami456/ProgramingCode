#include <DxLib.h>
#include"Scene/SceneManager.h"
#include"Scene/TitleScene.h"
#include"Input.h"

using namespace std;

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
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

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}