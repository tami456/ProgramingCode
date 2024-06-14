#pragma once
#include <vector>
#include <DxLib.h>

class ShotBase;
class Camera;

class Cannon
{
public:

	// �C�g��X��]����(�ő�)
	static constexpr float MAX_ANGLE_X = 15.0f * DX_PI_F / 180.0f;

	// �C�g��X��]����(�ŏ�)
	static constexpr float MIN_ANGLE_X = -30.0f * DX_PI_F / 180.0f;

	//�e���ˌ�̍d������
	static constexpr float SHOT_DELAY = 1.0f;

	//�����̃T�C�Y
	static constexpr int BLAST_SIZE_X = 32;
	static constexpr int BLAST_SIZE_Y = 32;

	//�����̃A�j���[�V������
	static constexpr int BLAST_ANIM_NUM = 16;

	Cannon();
	~Cannon();

	void Init();
	void Update();
	void Draw();
	void Release();

	std::vector<ShotBase*> GetShots(void);

private:

	VECTOR standScl_;
	VECTOR standRot_;
	VECTOR standPos_;

	VECTOR barrelScl_;
	VECTOR barrelRot_;
	VECTOR barrelPos_;

	//�C�䂩��̑��΍��W
	VECTOR barrelLocalPos_;


	// �R�c���f���̃n���h��ID
	int barrelModelId_;
	int standModelId_;

	//�V���b�g(�|�C���^)
	std::vector<ShotBase*> shots_;

	//�e�̃��f��ID
	int shotModelId_;
	int spikeShotModelId_;

	float stepShotDelay;

	//�����̉摜(�{���͊O���\�[�X�p�̊Ǘ��N���X�����ׂ��B�e�����f���B)
	int blastImgs_[BLAST_ANIM_NUM];

	void ProcessRot(void);

	//���ˑ���
	void ProcessShot(void);

	//�L���Ȓe���擾����
	ShotBase* GetValidShot(void);
};

