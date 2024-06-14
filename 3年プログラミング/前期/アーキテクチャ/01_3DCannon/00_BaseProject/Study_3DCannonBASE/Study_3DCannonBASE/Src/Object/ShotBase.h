#pragma once
#include<DxLib.h>

class ShotBase
{
public:

	static constexpr float COL_RADIUS = 10.0f;

	//�e�̏��
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	// �R���X�g���N�^(���ƂȂ郂�f���̃n���h��ID)
	ShotBase(int baseModelId,int* blastImg,int blastAnimNum);

	// �f�X�g���N�^
	virtual ~ShotBase(void);

	// �e�̐���(�\���J�n���W�A�e�̐i�s����)
	void CreateShot(VECTOR pos, VECTOR dir);

	// �X�V�X�e�b�v
	void Update(void);
	void UpdateShot(void);
	void UpdateBlast(void);
	void UpdateEND(void);

	// �`��
	void Draw();
	void DrawShot(void);
	void DrawBlast(void);
	void DrawEND(void);

	// �������
	void Release(void);

	//�e����
	bool IsShot(void);

	//��������
	void Blast(void);

	// ��������
	bool IsAlive(void);

	VECTOR GetPos(void) const;

private:

	//�e�̏��
	STATE state_;

	// ���ƂȂ�e�̃��f��ID
	int baseModelId_;

	// �e�̃��f��ID
	int modelId_;

	// ����
	VECTOR dir_;

	// �e�̑傫��
	VECTOR scl_;

	// �e�̊p�x
	VECTOR rot_;

	// �e�̍��W
	VECTOR pos_;

	// �e�̈ړ����x
	float speed_;

	// �e�̐�������
	bool isAlive_;

	//�d��
	float gravityPow_;

	//�����A�j���[�V�����摜�z��̃|�C���^
	int* blastImgs_;

	//�����A�j���[�V������
	int blastAnimNum_;

	//�����̃A�j���[�V�����p�J�E���^
	int blastCntAnim_;

	//�����A�j���[�V�������x
	float blastSpeedAnim_;

	//�����̃A�j���[�V�����ԍ�
	int blastIdxAnim_;

	//��ԑJ��
	void ChangeState(STATE state);
};

