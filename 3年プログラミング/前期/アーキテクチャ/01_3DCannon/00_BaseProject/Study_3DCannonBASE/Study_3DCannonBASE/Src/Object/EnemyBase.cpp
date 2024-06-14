#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(int baseModelId)
{
    baseModelId_ = baseModelId;
}

EnemyBase::~EnemyBase(void)
{
}

void EnemyBase::Init(void)
{
    SetParam();
    Update();
}

void EnemyBase::SetParam(void)
{
    //�g�p�������e�ʂƓǂݍ��ݎ��ԍ팸�̂���
    //���f���f�[�^����������������ɑ��݂����Ȃ�
    modelId_ = MV1DuplicateModel(baseModelId_);

    //�傫���̐ݒ�
    scl_ = { 0.3f,0.3f,0.3f };

    //�p�x�̐ݒ�
    rot_ = { 0.0f,-90.0f * DX_PI_F / 180.0f,0.0f };

    // �ʒu�̐ݒ�
    pos_ = { -350.0f, 30.0f, 75.0f };

    // �E�����Ɉړ�����
    dir_ = { 1.0f, 0.0f, 0.0f };

    // �ړ��X�s�[�h
    speed_ = 1.5f;

    // �����͐������
    isAlive_ = true;

    //�A�j���[�V�������A�^�b�`����
    animAttachNo_ = MV1AttachAnim(modelId_, 10);

    //�A�^�b�`���Ă���A�j���[�V�����̑��ِ����Ԃ��擾����
    animTotalTime_ = MV1GetAttachAnimTotalTime(modelId_, animAttachNo_);

    //�Đ����̃A�j���[�V��������
    stepAnim_ = 0.0f;

    //�A�j���[�V�������x
    speedAnim_ = 30.0f;

    //HP�̐ݒ�
    hp_ = hpMax_ = 2;

    //�Փ˔���p�̋��̔��a
    collisionRadius_ = 35.0f;

    //�Փ˔���p�̋��̒��S�̒������W
    collisionLocalPos_ = { 0.0f,50.0f,0.0f };

    collisionPos_ = { 0.0f,0.0f,0.0f };

}

void EnemyBase::Update(void)
{
    if (!IsAlive())
    {
        return;
    }

    MV1SetScale(modelId_, scl_);
    MV1SetRotationXYZ(modelId_, rot_);
    MV1SetPosition(modelId_, pos_);

    VECTOR movePow;
    movePow = VScale(dir_,speed_);
    pos_ = VAdd(pos_, movePow);
    collisionPos_ = VAdd(pos_, collisionLocalPos_);
    //�A�j���[�V�����Đ�

    //�o�ߎ��Ԃ̎擾
    float deltaTime = 1.0f / SceneManager::DEFFAULT_FPS;

    //�A�j���[�V�����̎��ԐU��
    stepAnim_ += (speedAnim_ * deltaTime);

    if (stepAnim_ > animTotalTime_)
    {
        //���[�v�Đ�
        stepAnim_ = 0.0f;
    }

    //�Đ�����A�j���[�V�������Ԃ̐ݒ�
    MV1SetAttachAnimTime(modelId_, animAttachNo_, stepAnim_);

    if (hp_ <= 0)
    {
        MV1DeleteModel(modelId_);
        isAlive_ = false;
    }
}

void EnemyBase::Draw(void)
{
    
    MV1DrawModel(modelId_);
    VECTOR pos = pos_;
    pos = VAdd(pos, collisionLocalPos_);
    DrawSphere3D(collisionPos_, collisionRadius_, 10,0xff0000, 0xff0000, false);
}

void EnemyBase::Release(void)
{
    MV1DeleteModel(modelId_);
}

VECTOR EnemyBase::GetPos(void)
{
    return pos_;
}

void EnemyBase::SetPos(VECTOR pos)
{
    pos_ = pos;
}

bool EnemyBase::IsAlive(void)
{
    return isAlive_;
}

void EnemyBase::SetAlive(bool alive)
{
    isAlive_ = alive;
}

void EnemyBase::Damage(int damage)
{
    hp_ -= damage;
}

VECTOR EnemyBase::GetCollisionPos(void)
{
    return collisionPos_;
}

float EnemyBase::GetCollisionRadius(void)
{
    return collisionRadius_;
}
