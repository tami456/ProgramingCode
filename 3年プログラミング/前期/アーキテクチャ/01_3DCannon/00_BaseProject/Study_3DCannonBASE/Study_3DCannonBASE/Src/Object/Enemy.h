#pragma once
#include "EnemyBase.h"
class Enemy :
    public EnemyBase
{
public:
    Enemy(int baseModelId);
    ~Enemy();

	// ��������(�ŏ��̂P��̂ݎ��s)
	void Init(void) override;

	// �p�����[�^�ݒ�(�������z�֐�)
	void SetParam(void) override;

	// �X�V����(���t���[�����s)
	void Update(void) override;

	// �`�揈��(���t���[�����s)
	void Draw(void) override;

	// �������(�Ō�̂P��̂ݎ��s)
	void Release(void) override;

private:
};

