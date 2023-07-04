#include <dxLib.h>
#include "Item.h"
#include "../Scene/GameScene.h"



Item::Item(ActorType type, Vector2D pos):Actor(type,pos)
{

}

Item::~Item()
{

}

void Item::Init()
{
	itemFlag = false;
}

void Item::Update(BaseScene& scene)
{
	
	
	if (!itemFlag)
	{
		pos_.x = rand() % (600 - 16);
		pos_.y = -16;
		itemFlag = true;
	}

	if (itemFlag)
	{
		pos_.y += speed_;
		if (pos_.y >= 500)
		{
			itemFlag = false;
		}
	}
	
}
bool Item::CheckHit(const ActorList& actList)
{
	for (auto actor : actList)
	{
		if ((actor->GetActorType() == ActorType::Player))
		{
			if (actor->GetPos() == pos_)
			{
				itemFlag = false;
				animeCtl_.SetState(State::Death);
				return true;
			}
		}
	}
	return false;
}



