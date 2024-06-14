#pragma once
#include <DxLib.h>
class Camera
{
public:

	//�e���ˌ�̍d������
	static constexpr float SHOT_DELAY = 1.0f;

	//�J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,	//��_�J����
		FREE,			//�t���[���[�h
	};

	// �R���X�g���N�^
	Camera(void);

	// �f�X�g���N�^
	~Camera();

	// ��������(��{�I�ɍŏ��̂P�񂾂�����)
	void Init(void);

	// �X�V����(���t���[�����s)
	void Update(void);

	// �J�����ݒ�(���t���[�����s)
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);

	// �`�揈��(���t���[�����s)
	void Draw(void);

	// �������(��{�I�ɍŌ�̂P�񂾂�����)
	void Release(void);

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	void SetPos(VECTOR pos);

private:

	// �J�������[�h
	MODE mode_;


	// �J�����̈ʒu
	VECTOR pos_;

	float dx;
	float stepShotDelay;

	// �J�����̊p�x
	VECTOR angles_;

	
};

