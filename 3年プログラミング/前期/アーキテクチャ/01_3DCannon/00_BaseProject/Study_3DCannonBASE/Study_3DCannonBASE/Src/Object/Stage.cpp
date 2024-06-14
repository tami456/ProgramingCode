#include <DxLib.h>
#include "../Application.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	// �O���t�@�C���̂R�c���f�������[�h
	modelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Stage/Stage.mv1").c_str());

	// �R�c���f���̑傫����ݒ�(�����́Ax, y, z�̔{��)
	MV1SetScale(modelId_, { 1.0f, 1.0f, 1.0f });

	// �R�c���f���̈ʒu(�����́A�R�c���W)
	MV1SetPosition(modelId_, { 0.0f, 0.0f, 0.0f });

	// �R�c���f���̌���(�����́Ax, y, z�̉�]�ʁB�P�ʂ̓��W�A���B)
	MV1SetRotationXYZ(modelId_, { 0.0f, DX_PI_F, 0.0f });

	//�Փ˔�����
	MV1SetupCollInfo(modelId_);

	//�w�i�摜�ǂݍ���
	imgBack_ = LoadGraph((Application::PATH_IMAGE + "Sky.jpg").c_str());
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	// ���[�h���ꂽ�R�c���f������ʂɕ`��
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
	//�摜�̊J��
	DeleteGraph(imgBack_);

	// ���[�h���ꂽ�R�c���f����������������
	MV1DeleteModel(modelId_);
}

int Stage::GetModelId(void) const
{
	return modelId_;
}
