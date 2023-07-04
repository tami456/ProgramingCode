#pragma once
#include "Actor.h"
class Bomb :
    public Actor
{
public:
	Bomb(ActorType type, Vector2D pos);
	~Bomb();
private:
	void Update(BaseScene& scene) override;
	int count_;
};

