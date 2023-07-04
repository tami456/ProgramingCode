#pragma once
#include "Player.h"
class Enemy :
	public Player
{
public:
	Enemy(ActorType type, Vector2D pos);
	~Enemy();
private:
	//void Update(BaseScene& scene) override;
};

