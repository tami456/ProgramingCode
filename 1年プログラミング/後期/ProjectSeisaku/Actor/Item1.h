#pragma once
#include "Actor.h"
class Item1 :
    public Actor
{
public:
	Item1(ActorType type, Vector2D pos);
	~Item1();
	void Init();
	void Update(BaseScene& scene)override;
private:
	unsigned int itemFlag;
};

