#include <dxlib.h>
#include "Actor.h"
#include "../common/AnimeCtl.h"
Actor::Actor()
{
	type_ = ActorType::Player;
	
}

Actor::Actor(ActorType type, Vector2D pos)
{
	type_ = type;
	pos_ = pos;
	speed_ = 2;
	alive_ = false;
	bombPair_ = { false,pos_ };
}

Actor::~Actor()
{
}

void Actor::Draw(const Vector2D& camOffset)
{

	animeCtl_.Draw(camOffset);
}

void Actor::LoadAnime(void)
{
	animeCtl_ = AnimeLoader().execute(type_);		//ÇªÇÃèÍÇ≈égÇ¡Çƒè¡Ç¶ÇƒÇ¢Ç¢
}

bool Actor::CheckErase(void)
{
	if (animeCtl_.GetState() == State::Dead)
	{
		if (animeCtl_.IsAnimeEnd())
		{
			alive_ = false;
		}
		return true;
	}
	return false;
}
