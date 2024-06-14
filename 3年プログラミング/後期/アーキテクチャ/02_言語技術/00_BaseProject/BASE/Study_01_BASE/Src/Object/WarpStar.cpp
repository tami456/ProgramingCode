#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "Player.h"
#include "WarpStar.h"

WarpStar::WarpStar(
	Player& player, const Transform& transform):player_(player)
{
	
	/*player_ = player;*/

	transform_ = transform;

	state_ = STATE::NONE;

}

WarpStar::~WarpStar(void)
{
}

void WarpStar::Init(void)
{

	// ���f���̊�{���
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::WARP_STAR)
	);
	transform_.Update();

	ChangeState(STATE::IDLE);

}

void WarpStar::Update(void)
{

	// �X�V�X�e�b�v
	switch (state_)
	{
	case WarpStar::STATE::NONE:
		UpdateNone();
		break;
	case WarpStar::STATE::IDLE:
		UpdateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		UpdateReserve();
		break;
	case WarpStar::STATE::MOVE:
		UpdateMove();
		break;
	}

}

void WarpStar::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void WarpStar::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case WarpStar::STATE::NONE:
		ChangeStateNone();
		break;
	case WarpStar::STATE::IDLE:
		ChangeStateIdle();
		break;
	case WarpStar::STATE::RESERVE:
		ChangeStateReserve();
		break;
	case WarpStar::STATE::MOVE:
		ChangeStateMove();
		break;
	}

}

void WarpStar::ChangeStateNone(void)
{
}

void WarpStar::ChangeStateIdle(void)
{
}

void WarpStar::ChangeStateReserve(void)
{
}

void WarpStar::ChangeStateMove(void)
{
}

void WarpStar::UpdateNone(void)
{
}

void WarpStar::UpdateIdle(void)
{
}

void WarpStar::UpdateReserve(void)
{
}

void WarpStar::UpdateMove(void)
{
}
