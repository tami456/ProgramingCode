#include "Bomb.h"
#include "../Scene/FireMap.h"
#include "../Scene/GameScene.h"
Bomb::Bomb(ActorType type, Vector2D pos) :
	Actor(type, pos)
{
	count_ = 180;
}

Bomb::~Bomb()
{
}

void Bomb::Update(BaseScene& scene)
{
	if (CheckErase())
	{
		return;
	}
	auto fireMap = dynamic_cast<GameScene&>(scene).GetFireMap();
	if (fireMap.expired())
	{
		return;
	}
	auto lockFireMap = fireMap.lock();
	if (lockFireMap->CheckFire(pos_))
	{
		count_ = 1;
	}
	if (--count_)
	{
		return;
	}
	animeCtl_.SetState(State::Dead);


	lockFireMap->SetData(pos_, 3);
}