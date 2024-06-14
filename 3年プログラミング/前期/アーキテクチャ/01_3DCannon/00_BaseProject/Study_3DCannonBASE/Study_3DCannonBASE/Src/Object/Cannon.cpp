#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "ShotBase.h"
#include "../Manager/Camera.h"
#include "Cannon.h"

Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

void Cannon::Init()
{
	standScl_ = { 0.8f,0.8f,0.8f };
	standRot_ = { 0.0f,0.0f,0.0f };
	standPos_ = { 0.0f,10.0f,-200.0f };

	barrelScl_ = { 0.8f,0.8f,0.8f };
	barrelRot_ = { 0.0f,0.0f,0.0f };
	barrelPos_ = { 0.0f,0.0f,0.0f };


	//�ʒu�̐ݒ�
	//�y�䂩��̑��΍��W�Ƃ���
	barrelLocalPos_ = { 0.0f,120.0f,0.0f };

	barrelPos_ = VAdd(standPos_, barrelLocalPos_);

	// �O���t�@�C���̂R�c���f�������[�h
	barrelModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Barrel.mv1").c_str());
	standModelId_ = MV1LoadModel(
		(Application::PATH_MODEL + "Cannon/Stand.mv1").c_str());

	//�e�̃��f��
	shotModelId_ =
		MV1LoadModel((Application::PATH_MODEL + "Cannon/Shot.mv1").c_str());
	
	spikeShotModelId_ =
		MV1LoadModel((Application::PATH_MODEL + "Cannon/SpikeShot.mv1").c_str());

	//�����G�t�F�N�g�ǂݍ���
	LoadDivGraph((Application::PATH_IMAGE + "Blast.png").c_str(),
		BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

	//�e���˂̍d������
	stepShotDelay = 0.0f;

	//�����ݒ�����f���ɔ��f(�ŏ��͎������Ȃ�)
	Update();
}

void Cannon::Update()
{
	

	MV1SetPosition(standModelId_, standPos_);
	MV1SetRotationXYZ(standModelId_, standRot_);
	MV1SetPosition(barrelModelId_, barrelPos_);
	MV1SetRotationXYZ(barrelModelId_, barrelRot_);
	for (auto shot : shots_)
	{
		shot->Update();
	}
	ProcessRot();
	ProcessShot();

	
}

void Cannon::Draw()
{
	MV1DrawModel(standModelId_);
	
	//���j�̃��f���`��
	MV1DrawModel(barrelModelId_);

	for (auto shot : shots_)
	{
		shot->Draw();
	}

}

void Cannon::Release()
{
	MV1DeleteModel(barrelModelId_);
	MV1DeleteModel(standModelId_);
	MV1DeleteModel(shotModelId_);
	MV1DeleteModel(spikeShotModelId_);

	for (auto shot : shots_)
	{
		shot->Release();
		delete shot;
	}

	// �ǂݍ��񂾉摜�̉��
	for (int i = 0; i < BLAST_ANIM_NUM; i++)
	{
		DeleteGraph(blastImgs_[i]);
	}
}

std::vector<ShotBase*> Cannon::GetShots(void)
{
	return shots_;
}

void Cannon::ProcessRot()
{
	auto& ins = InputManager::GetInstance();
	float barrelRad = 1.0 * DX_PI_F / 180.0f;
	if (ins.IsNew(KEY_INPUT_RIGHT)) 
	{
		barrelRot_.y = standRot_.y  += 1.0 * DX_PI_F / 180.0f;
	}

	if(ins.IsNew(KEY_INPUT_LEFT))
	{
		barrelRot_.y = standRot_.y -= 1.0 * DX_PI_F / 180.0f;
	}


	if (ins.IsNew(KEY_INPUT_UP)) 
	{ 
		barrelRot_.x += barrelRad; 
		if (barrelRot_.x > MAX_ANGLE_X)
		{
			barrelRot_.x = MAX_ANGLE_X;
		}
	}

	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		barrelRot_.x -= barrelRad;
		if (barrelRot_.x < MIN_ANGLE_X)
		{
			 barrelRot_.x = MIN_ANGLE_X;
		}
	}

	
}

void Cannon::ProcessShot(void)
{
	// �e�̔��ˈʒu
	VECTOR pos = barrelPos_;
	// �C�g����̑��΍��W
	VECTOR localPos = { 0.0f, 25.0f, 30.0f };

	

	//�P�ʍs��(����]�̏��)
	MATRIX matRot = MGetIdent();

	//���ꂼ��̎��̍s�������A����ɍs����������Ă���
	matRot = MMult(matRot, MGetRotX(barrelRot_.x));
	matRot = MMult(matRot, MGetRotY(barrelRot_.y));
	matRot = MMult(matRot, MGetRotZ(barrelRot_.z));

	//��]�s����g�p���āA�x�N�g������]������
	VECTOR dir = VTransform({ 0.0f,0.0f,1.0f }, matRot);

	// �����Ɠ����v�̂ŁA���΍��W����]
	VECTOR localPosRot = VTransform(localPos, matRot);

	pos = VAdd(pos, localPosRot);

	auto& ins = InputManager::GetInstance();

	//�U���L�[�������ƁA�e�𐶐�
	if (ins.IsNew(KEY_INPUT_Z) && stepShotDelay <= 0.0f)
	{
		//�L���ȋ����擾����
		ShotBase* shot = GetValidShot();

		//�e�𐶐�(�����͉��Ő��ʕ���)
		shot->CreateShot(pos, dir);

		//�ʔ��ˌ�̍d�����ԃZ�b�g
		stepShotDelay = SHOT_DELAY;
	}

	//�e���ˌ�̍d�����Ԃ����炵�Ă���
	if (stepShotDelay > 0.0f)
	{
		stepShotDelay -= 1.0f / SceneManager::DEFFAULT_FPS;
	}
}

ShotBase* Cannon::GetValidShot(void)
{
	size_t size = shots_.size();
	for (int i = 0; i < size; i++)
	{
		if (!shots_[i]->IsAlive())
		{
			return shots_[i];
		}
	}

	ShotBase* shot = new ShotBase(shotModelId_, blastImgs_, BLAST_ANIM_NUM);
	
	shots_.push_back(shot);

	return shot;
}

