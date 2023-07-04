#pragma once
#include "Player.h"
#include "Actor.h"
#include "../common/Vector2D.h"

class Item :
	public Actor
{
public:
	Item(ActorType type, Vector2D pos);
	~Item();
	void Init();
	void Update(BaseScene& scene)override;
	bool CheckHit(const ActorList& actList);
private:
	unsigned int itemFlag;
};

