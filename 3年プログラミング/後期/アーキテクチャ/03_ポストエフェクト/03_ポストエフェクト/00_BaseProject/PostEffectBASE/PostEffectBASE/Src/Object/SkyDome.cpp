#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(const Transform& syncTransform) : syncTransform_(syncTransform)
{

	state_ = STATE::NONE;
	
	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&SkyDome::ChangeStateNone, this));
	stateChanges_.emplace(STATE::STAY, std::bind(&SkyDome::ChangeStateStay, this));
	stateChanges_.emplace(STATE::FOLLOW, std::bind(&SkyDome::ChangeStateFollow, this));

}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	transform_.scl = SCALES;
	transform_.pos = AsoUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f, 
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	// ��ԑJ��
	auto sceneId = scnMng_.GetSceneID();
	if(sceneId == SceneManager::SCENE_ID::TITLE)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}

}

void SkyDome::Update(void)
{

	// �X�V�X�e�b�v
	stateUpdate_();

}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

}

void SkyDome::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&SkyDome::UpdateNone, this);
}

void SkyDome::ChangeStateStay(void)
{
	stateUpdate_ = std::bind(&SkyDome::UpdateStay, this);
}

void SkyDome::ChangeStateFollow(void)
{

	stateUpdate_ = std::bind(&SkyDome::UpdateFollow, this);

	transform_.pos = syncTransform_.pos;
	transform_.Update();

}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{
}

void SkyDome::UpdateFollow(void)
{
	transform_.pos = syncTransform_.pos;
	transform_.Update();
}
