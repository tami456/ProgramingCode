#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "ShotBase.h"

ShotBase::ShotBase(int baseModelId, int* blastImgs, int blastAnimNum)
{
    baseModelId_ = baseModelId;
    blastImgs_ = blastImgs;
    blastAnimNum_ = blastAnimNum;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
    //�g�p�������e�ʂƓǂݍ��ݎ��ԍ팸�̂���
    //���f���f�[�^����������������ɑ��݂����Ȃ�
    modelId_ = MV1DuplicateModel(baseModelId_);

    //�e�̑傫����ݒ�
    scl_ = { 0.8f,0.8f,0.8f };

    //�e�̊p�x��ݒ�
    rot_ = { 0.0f,0.0f,0.0f };

    //�e�̔��ˈʒu��ݒ�
    pos_ = pos;

    //�e�̔��˕����̐ݒ�
    dir_ = dir;

    //�e�̑��x
    speed_ = 8.0f;

    //�e�̐�������
    isAlive_ = true;

    //�d��
    gravityPow_ = 0.0f;

    //�����̃A�j���[�V�����p�J�E���^
    blastCntAnim_ = 0;

    //�����̃A�j���[�V�������x
    blastSpeedAnim_ = 0.3f;

    //��ԑJ��
    ChangeState(STATE::SHOT);
}

void ShotBase::Update(void)
{
    //�傫���̐ݒ�
    MV1SetScale(modelId_, scl_);

    //�p�x�̐ݒ�
    MV1SetRotationXYZ(modelId_, rot_);

    //�ʒu�̐ݒ�
    MV1SetPosition(modelId_, pos_);

    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        UpdateShot();
        break;
    case ShotBase::STATE::BLAST:
        UpdateBlast();
        break;
    case ShotBase::STATE::END:
        UpdateEND();
        break;
    }

}

void ShotBase::UpdateShot(void)
{
    if (!IsAlive())
    {
        //�������Ă��Ȃ���Ώ������f
        return;
    }

    //�e���ړ�������

    //�ړ��ʂ̌v�Z(�����~�X�s�[�h)
    VECTOR movePow;
    movePow = VScale(dir_, speed_);

    //�ړ�����(���W�{�ړ���)
    pos_ = VAdd(pos_, movePow);

    //����ɉ����x�I�ɏd�͂�������
    gravityPow_ += SceneManager::GRAVITY / SceneManager::DEFFAULT_FPS;
    pos_.y -= gravityPow_;
}

void ShotBase::UpdateBlast(void)
{
    blastIdxAnim_++;
    //�����A�j���[�V�����̏I������
    if (blastIdxAnim_ + 1 >= blastAnimNum_)
    {
        blastIdxAnim_ = 0;
        ChangeState(STATE::END);
    }
}

void ShotBase::UpdateEND(void)
{
    //�������Ȃ�
}

void ShotBase::Draw()
{
    switch (state_)
    {
    case ShotBase::STATE::SHOT:
        DrawShot();
        break;
    case ShotBase::STATE::BLAST:
        DrawBlast();
        break;
    case ShotBase::STATE::END:
        DrawEND();
        break;
    }

}

void ShotBase::DrawShot(void)
{
    if (!IsAlive())
    {
        //�������Ă��Ȃ���Ώ������f
        return;
    }
    MV1DrawModel(modelId_);

}

void ShotBase::DrawBlast(void)
{
    
    DrawBillboard3D(
        pos_, 0.5f, 0.5f, 80.0f, 0.0f, blastImgs_[blastIdxAnim_], true);
}

void ShotBase::DrawEND(void)
{
}

void ShotBase::Release(void)
{
    MV1DeleteModel(modelId_);
}

bool ShotBase::IsShot(void)
{

    if (state_ == STATE::SHOT)
    {
        return true;
    }

    return false;
}

void ShotBase::Blast(void)
{
    state_ = STATE::BLAST;
}

bool ShotBase::IsAlive(void)
{
    return state_ != STATE::END;
}

VECTOR ShotBase::GetPos(void) const
{
    return pos_;
}

void ShotBase::ChangeState(STATE state)
{
    state_ = state;

    switch (state_)
    {
    case ShotBase::STATE::NONE:
        break;
    case ShotBase::STATE::SHOT:
        break;
    case ShotBase::STATE::BLAST:
        break;
    case ShotBase::STATE::END:
        break;
    }
}
