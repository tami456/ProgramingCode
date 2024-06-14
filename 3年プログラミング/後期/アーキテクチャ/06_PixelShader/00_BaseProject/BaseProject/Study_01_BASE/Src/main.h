#pragma once

// �X�N���[���T�C�Y
const int SCREEN_X = 1080;
const int SCREEN_Y = 800;
// �C���[�W�T�C�Y
const int IMG_SIZE_X = 128;
const int IMG_SIZE_Y = 128;
const int IMG_SIZE_HX = IMG_SIZE_X / 2;
const int IMG_SIZE_HY = IMG_SIZE_Y / 2;
// �ʒu����
const int PLUS_X = 150;
const int TITLE_Y = -20;
// �V�F�[�_�[�p�X
const std::string PATH_SHADER = "x64/Debug/";
// �f���^�^�C��
std::chrono::system_clock::time_point mPreTime;
float mDeltaTime;
float mTotalTime;
// �`��ʒu
int mPosX;
int mPosY;
// ���_���
VERTEX2DSHADER mVertex[4];
WORD mIndex[6];
// �X�N���[���n���h��
int mainScreen;
// �摜�n���h��
int texDragon;
int texDragonNor;
int texFire;
int texNoize;
int texWhite;

// �V�F�[�_�[�n���h��

void Init(void);
void Release(void);
void Run(void);
// �`��p�̎l�p���_���쐬
void MakeSquereVertex(void);
// ���s
void NewLine(void);
// �^�C�g���`��
void DrawTitle(std::string title);
// �g�`��
void DrawFrame(void);
// �V�F�[�_�[�`��֐�

