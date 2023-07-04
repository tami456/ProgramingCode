#include <string>
#include <EffekseerForDxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Common/SpeechBalloon.h"
#include "Planet.h"
#include "Player.h"
#include "Enemy.h"
#include "Mascot.h"

Mascot::Mascot() : mResourceManager(ResourceManager::GetInstance())
{
	mAnimationController = nullptr;
	ImgMascotIdle = -1;
	ImgMascotNice = -1;
	mState = STATE::NONE;
}

Mascot::~Mascot()
{
}

void Mascot::Init(void)
{
	ResourceManager& rm = ResourceManager::GetInstance();

	// ÉÇÉfÉãÇÃäÓñ{ê›íË
	ImgMascotIdle = rm.Load(ResourceManager::SRC::MASCOT).mHandleId;
	ImgMascotNice = rm.Load(ResourceManager::SRC::MASCOTNICE).mHandleId;

	//èâä˙èÛë‘
	ChangeState(STATE::PLAY);
}

void Mascot::Update(void)
{

	switch (mState)
	{
	case Mascot::STATE::NONE:
		break;
	case Mascot::STATE::PLAY:
		break;
	case Mascot::STATE::IDLE:
		UpdateIdle();
		break;
	case Mascot::STATE::ENEMYDEAD:
		break;
	}

}

void Mascot::UpdateIdle(void)
{

}

void Mascot::UpdateEnemyDead(void)
{
}

void Mascot::Draw(void)
{
	DrawGraph(0, 0, ImgMascotIdle, false);
}

void Mascot::Release(void)
{

}

void Mascot::ChangeState(STATE state)
{
	mState = state;
	switch (mState)
	{
	case Mascot::STATE::NONE:
		break;
	case Mascot::STATE::PLAY:
		break;
	case Mascot::STATE::IDLE:
		UpdateIdle();
		break;
	case Mascot::STATE::ENEMYDEAD:
		break;
	}
}
