#pragma once
#include "Dir.h"
#include "State.h"
#include "Actor.h"
#include "../Input/Controller.h"
#include "Item.h"


class Player:
	public Actor
{
public:
	Player(ActorType type, Vector2D pos);
	~Player();
	void Init();
	void Update(BaseScene& scene)override;
	

private:
	int playerAnimCount;
protected:
	std::unique_ptr<Controller> controller_;
};

