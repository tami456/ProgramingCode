#include <dxlib.h>
#include "Actor.h"

Actor::Actor(ActorType type, Vector2D pos)
{
	type_ = type;
	pos_ = pos;
	speed_ = 2;
	alive_ = true;
}

Actor::~Actor()
{
}

void Actor::Draw(void)
{

	animeCtl_.Draw();
	
}

void Actor::LoadAnime(void)
{
	animeCtl_ = AnimeLoader().execute(type_);
}
