#pragma once
#include <memory>
#include <list>
#include "BaseScene.h"
#include "../Input/Controller.h"
#include "../Actor/ActorType.h"
#include "../Actor/Actor.h"
#include "../Actor/Player.h"
#include "../Actor/Stamina.h"
#include "ResultScene.h"
#include "../common/Bg.h"
#include "../Actor/Item.h"
#include "../Actor/Item1.h"

class Controller;

class GameScene:
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	bool Init(void);
	uniqueScene Update(uniqueScene ownScene)override;
	void DrawOwnScreen(void)override;
	const ActorList GetActorList(void) { return actorList_; }
private:
	bool CreateActor(ActorType type, Vector2D pos);

	template<class MyActor>
	bool CreateActor(ActorType type, Vector2D pos, bool poolOnly);
	Bg bg_;
	Stamina stamina_;
	Vector2D chipSize_;
	ActorList actorList_;
	std::map<ActorType, std::unique_ptr<Actor>> actorPool_;
	int sH;
	int volume;
	int gage;
	float gage2;
	int point;
	int stamina;
	int get;
};

template<class MyActor>
inline bool GameScene::CreateActor(ActorType type, Vector2D pos, bool poolOnly)
{
	if (!actorPool_.count(type))
	{
		actorPool_.try_emplace(type, std::make_unique<MyActor>(type, pos));
		actorPool_[type]->LoadAnime();
		actorPool_[type]->animeCtl_.reConnect(&actorPool_[type]->pos_);
		actorPool_[type]->animeCtl_.SetState(State::Run);
	}
	if (poolOnly)
	{
		return true;
	}
	actorList_.emplace_back(std::make_unique<MyActor>(type, pos));
	auto actor = (--actorList_.end());
	(*actor)->alive_ = true;							//pool‚©‚ço‚½‚ç¶‚«‚Ä‚¢‚é
	(*actor)->animeCtl_ = actorPool_[type]->animeCtl_;
	(*actor)->animeCtl_.reConnect(&(*actor)->pos_);
	

	return true;

}