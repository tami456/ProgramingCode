#include "Item1.h"

Item1::Item1(ActorType type, Vector2D pos):Actor(type,pos)
{
}

Item1::~Item1()
{
}

void Item1::Init()
{
	itemFlag = false;
}

void Item1::Update(BaseScene& scene)
{
	if (!itemFlag)
	{
		pos_.x = rand() % (600 - 16);
		pos_.y = -700;
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
