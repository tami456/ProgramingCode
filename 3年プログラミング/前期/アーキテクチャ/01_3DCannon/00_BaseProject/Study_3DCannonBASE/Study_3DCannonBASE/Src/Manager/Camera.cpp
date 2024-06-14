#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	//�J�����̈ʒu
	pos_ = { 0.0f,400.0f,-400.0f };

	dx = 0.0f;

	stepShotDelay = 0.0f;

	//�J�����̊p�x
	angles_ = { 40.0f * DX_PI_F / 180.0f,0.0f,0.0f };

	//��_�J������������Ԃɂ���
	ChangeMode(MODE::FIXED_POINT);
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{
	//�N���b�v������ݒ肷��
	SetCameraNearFar(10.0f, 30000.0f);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	}

	//�J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//�������Ȃ�
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	//WASD�ŃJ�����̈ʒu��ς���
	float movePow = 3.0f;

 	if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; }
	if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; }
	if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; }
	if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; }
	if (ins.IsNew(KEY_INPUT_Q)) { pos_.y -= movePow; }
	if (ins.IsNew(KEY_INPUT_E)) { pos_.y += movePow; }

	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay <= 0.0f)
	{ 
		dx = 10.0f; 
		//�ʔ��ˌ�̍d�����ԃZ�b�g
		stepShotDelay = SHOT_DELAY;
	};

	//�e���ˌ�̍d�����Ԃ����炵�Ă���
	if (stepShotDelay > 0.0f)
	{
		stepShotDelay -= 1.0f / SceneManager::DEFFAULT_FPS;
	}

	if (fabsf(dx) > 0.0f)
	{
		dx = -0.9f * dx;
	}
	SetPos(VGet(dx, pos_.y, pos_.z));

	// ���L�[�ŃJ�����̊p�x��ς���
	/*float rotPow = 1.0f * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_DOWN)) { angles_.x -= rotPow; }
	if (ins.IsNew(KEY_INPUT_UP)) { angles_.x += rotPow; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }*/
}

void Camera::Draw(void)
{

	DrawFormatString(0, 0, 0x000000,
		"�J�������W�F(% .2f, % .2f, % .2f)", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 20, 0x000000, "�J�����p�x�F(%.2f, %.2f, %.2f)",
		angles_.x * 180.0f / DX_PI_F,
		angles_.y * 180.0f / DX_PI_F,
		angles_.z * 180.0f / DX_PI_F);
}

void Camera::Release(void)
{
}

void Camera::ChangeMode(MODE mode)
{
	// �J�������[�h�̕ύX
	mode_ = mode;
	// �ύX���̏���������
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}

}

void Camera::SetPos(VECTOR pos)
{
	pos_ = pos;
}
