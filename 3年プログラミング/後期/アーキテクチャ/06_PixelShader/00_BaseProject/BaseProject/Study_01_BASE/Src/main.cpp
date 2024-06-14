#include <chrono>
#include <math.h>
#include <string>
#include <DxLib.h>
#include "main.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode(TRUE);
	SetGraphMode(SCREEN_X, SCREEN_Y, 32, 60);

	// �c�w���C�u�����̏�����
	if (DxLib_Init() < 0)
	{
		// �G���[�����������璼���ɏI��
		return -1;
	}

	// ������
	Init();
	// ���s
	Run();
	// ���
	Release();
	// �c�w���C�u�����̌�n��
	DxLib_End();
	// �\�t�g�̏I��
	return 0;
}

void Init(void)
{

	// �`��Ɏg�p����摜�̓ǂݍ���
	texDragon = LoadGraph("Data/Dragon.png");

	// �`��Ɏg�p����摜�̓ǂݍ���
	texDragonNor = LoadGraph("Data/Dragon_n.png");

	// �`��Ɏg�p����摜�̓ǂݍ���
	texFire = LoadGraph("Data/Fire.png");

	// �`��Ɏg�p����摜�̓ǂݍ���
	texNoize = LoadGraph("Data/Noise128.png");

	// �`��Ɏg�p����摜�̓ǂݍ���
	texWhite = LoadGraph("Data/White.png");

}

void Release(void)
{

	// �s�N�Z���V�F�[�_�[�����


	// �s�N�Z���V�F�[�_�[�p�萔�o�b�t�@�����


}

void Run(void)
{

	// ���C���X�N���[���쐬
	mainScreen = MakeScreen(SCREEN_X, SCREEN_Y, true);

	// �f���^�^�C��
	mPreTime = std::chrono::system_clock::now();
	mTotalTime = 0.0f;

	// ESC�L�[���������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		// �f���^�^�C��
		auto nowTime = std::chrono::system_clock::now();
		mDeltaTime = static_cast<float>(
			std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - mPreTime).count() / 1000000000.0);
		mPreTime = nowTime;

		// �Q�[���N������
		mTotalTime += mDeltaTime;

		// ���C���֐ؑ�
		SetDrawScreen(mainScreen);

		// ��ʂ�������
		ClearDrawScreen();

		// �w�i�F�̕`��
		DrawBox(0, 0, SCREEN_X, SCREEN_Y, 0x00aaaa, true);

		// ���W�̏�����
		mPosX = 10;
		mPosY = 80;
		
		// �ʏ�̕`��
		DrawTitle("DrawGraph");
		DrawGraph(mPosX, mPosY, texDragon, true);
		mPosX += PLUS_X;

		// ����ʂɕ`��
		SetDrawScreen(DX_SCREEN_BACK);
		// ��ʂ�������
		ClearDrawScreen();
		// �`��
		DrawGraph(0, 0, mainScreen, true);

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

	}

	// ���C���X�N���[���폜
	DeleteGraph(mainScreen);

}

void MakeSquereVertex(void)
{

	// ���񒸓_�f�[�^���쐬����͖̂��ʂł����A
	// �V�F�[�_�[�ǉ����̍�Ƃ����炷���߁A���t���[���쐬

	int cnt = 0;
	float sX = static_cast<float>(mPosX);
	float sY = static_cast<float>(mPosY);
	float eX = static_cast<float>(mPosX + IMG_SIZE_X - 1);
	float eY = static_cast<float>(mPosY + IMG_SIZE_Y - 1);

	// �S���_�̏�����
	for (int i = 0; i < 4; i++)
	{
		mVertex[i].rhw = 1.0f;
		mVertex[i].dif = GetColorU8(255, 255, 255, 255);
		mVertex[i].spc = GetColorU8(255, 255, 255, 255);
		mVertex[i].su = 0.0f;
		mVertex[i].sv = 0.0f;
	}

	// ����
	mVertex[cnt].pos = VGet(sX, sY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// �E��
	mVertex[cnt].pos = VGet(eX, sY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 0.0f;
	cnt++;

	// �E��
	mVertex[cnt].pos = VGet(eX, eY, 0.0f);
	mVertex[cnt].u = 1.0f;
	mVertex[cnt].v = 1.0f;
	cnt++;

	// ����
	mVertex[cnt].pos = VGet(sX, eY, 0.0f);
	mVertex[cnt].u = 0.0f;
	mVertex[cnt].v = 1.0f;

	/*
	�@�`�`�`�`�`�`
		0-----1
		|     |
		|     |
		3-----2
	�@�`�`�`�`�`�`
		0-----1
		|  �^
		|�^
		3
	�@�`�`�`�`�`�`
			  1
		   �^ |
		 �^   |
		3-----2
	�@�`�`�`�`�`�`
	*/


	// ���_�C���f�b�N�X
	cnt = 0;
	mIndex[cnt++] = 0;
	mIndex[cnt++] = 1;
	mIndex[cnt++] = 3;

	mIndex[cnt++] = 1;
	mIndex[cnt++] = 2;
	mIndex[cnt++] = 3;

}

/// <summary>
/// ���s
/// </summary>
/// <param name=""></param>
void NewLine(void)
{
	mPosX = 10;
	mPosY += 180;
}

void DrawTitle(std::string title)
{
	DrawString(mPosX, mPosY + TITLE_Y, title.c_str(), 0x000000);
}

void DrawFrame(void)
{
	DrawBox(mPosX - 1, mPosY - 1, mPosX + IMG_SIZE_X, mPosY + IMG_SIZE_Y, 0x000000, false);
}
